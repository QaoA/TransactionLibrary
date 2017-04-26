#include "CLTransactionalObject.h"
#include "CLTransactionalObjectLookupTable.h"
#include "SLUserObjectInfo.h"
#include "CLGarbageCollector.h"
#include "CLLogItemsSet.h"
#include "CLSnapShot.h"
#include "CLReadTransactionReadedObjects.h"
#include "NVMMalloc.h"
#include "CLLogArea.h"
#include <cstring>
#include <cassert>

TRANSACTIONLIB_NS_BEGIN

CLTransactionalObject::CLTransactionalObject(SLTransactionalObjectCreatArgs & args):
m_pOwner(args.m_owner),
m_pUserInfo(args.m_pUserObjectInfo),
m_pNVMAddress(args.m_pNVMUserObject),
m_openMode(OPEN_NONE)
{
	switch (args.m_openMode)
	{
	case OPEN_READ:
		m_openMode = OPEN_READ;
		break;
	case OPEN_READ_ONLY:
		break;
	case OPEN_WRITE:
		m_openMode = OPEN_WRITE;
		break;
	default:
		break;
	}
	m_TentativeVersion = MakeANewVersion(args.m_pNVMUserObject);
}

CLTransactionalObject * CLTransactionalObject::MakeObject(void * pArgs)
{
	assert(pArgs);
	return new CLTransactionalObject(*(SLTransactionalObjectCreatArgs *)(pArgs));
}

void CLTransactionalObject::ReleaseObject(CLTransactionalObject * pObject)
{
	assert(pObject);
	SLObjectVersion * pCurrentVersion = pObject->m_TentativeVersion;
	while (pCurrentVersion)
	{
		SLObjectVersion * pTmpVersion = pCurrentVersion;
		pCurrentVersion = pCurrentVersion->m_pNextVersion;
		ReleaseVersion(pTmpVersion);
	}
	if (pObject->m_openMode & OPEN_DELETE)
	{
		pObject->m_pUserInfo->m_objectReleaseFunc(pObject->m_pNVMAddress);
	}
	delete pObject;
}

CLTransactionalObject * CLTransactionalObject::ReadOnlyOpen(void * pNVMUserObject, SLUserObjectInfo * pUserObjectInfo)
{
	assert(pNVMUserObject && pUserObjectInfo);
	SLTransactionalObjectCreatArgs args = { nullptr,pNVMUserObject,pUserObjectInfo,OPEN_READ_ONLY };
	return CLTransactionalObjectLookupTable::GetInstance().GetOrCreate(pNVMUserObject, &args);
}

void CLTransactionalObject::ReadOnlyCommit()
{
	CLTransactionalObjectLookupTable::GetInstance().Put(m_pNVMAddress);
}

void CLTransactionalObject::ReadOnlyAbort()
{
	CLTransactionalObjectLookupTable::GetInstance().Put(m_pNVMAddress);
}

CLTransactionalObject * CLTransactionalObject::ReadOpen(void * pNVMUserObject, CLWriteTransaction * pOwner, SLUserObjectInfo * pUserObjectInfo)
{
	assert(pNVMUserObject && pOwner && pUserObjectInfo);
	SLTransactionalObjectCreatArgs args = { pOwner,pNVMUserObject,pUserObjectInfo,OPEN_READ };
	CLTransactionalObject * pObject = CLTransactionalObjectLookupTable::GetInstance().GetOrCreate(pNVMUserObject, &args);
	assert(pObject);
	bool ret = TryOccupyObject(pObject,pOwner);
	if (ret)
	{
		pObject->m_openMode &= OPEN_READ;
		return pObject;
	}
	CLTransactionalObjectLookupTable::GetInstance().Put(pObject->m_pNVMAddress);
	return nullptr;
}

void CLTransactionalObject::ReadCommit(CLWriteTransaction * pOwner)
{
	assert(pOwner);
	bool ret = m_pOwner.compare_exchange_strong(pOwner, nullptr);
	assert(ret == true);
	m_openMode &= OPEN_CLEAR_MASK;
	CLTransactionalObjectLookupTable::GetInstance().Put(m_pNVMAddress);
}

void CLTransactionalObject::ReadAbort(CLWriteTransaction * pOwner)
{
	assert(pOwner);
	bool ret = m_pOwner.compare_exchange_strong(pOwner, nullptr);
	assert(ret == true);
	m_openMode = OPEN_NONE;
	CLTransactionalObjectLookupTable::GetInstance().Put(m_pNVMAddress);
}

CLTransactionalObject * CLTransactionalObject::WriteOpen(void * pNVMUserObject, CLWriteTransaction * pOwner, SLUserObjectInfo * pUserObjectInfo)
{
	assert(pNVMUserObject && pOwner && pUserObjectInfo);
	SLTransactionalObjectCreatArgs args = { pOwner,pNVMUserObject,pUserObjectInfo,OPEN_WRITE };
	CLTransactionalObject * pObject = CLTransactionalObjectLookupTable::GetInstance().GetOrCreate(pNVMUserObject, &args);
	assert(pObject);
	bool ret = TryOccupyObject(pObject, pOwner);
	if (ret)
	{
		SLObjectVersion * pOldVersion = pObject->m_TentativeVersion;
		pObject->m_TentativeVersion = pObject->CloneANewVersion(pOldVersion);
		pObject->m_openMode &= OPEN_WRITE;
		return pObject;
	}
	CLTransactionalObjectLookupTable::GetInstance().Put(pObject->m_pNVMAddress);
	return nullptr;
}

void CLTransactionalObject::WriteCommit(CLLogItemsSet & itemsSet, LSATimeStamp commitTime)
{
	if (m_openMode & OPEN_DELETE)
	{
		if (m_openMode & OPEN_NEW)
		{
			return;
		}

		unsigned int * pObjectReferenceCount = NVMMalloc::GetReferenceCountAddress(m_pNVMAddress);
		assert(pObjectReferenceCount);
		unsigned int rfcnt = 0;
		itemsSet.AddItem(pObjectReferenceCount, rfcnt);
		return;
	}
	
	m_TentativeVersion->m_commitTime = commitTime;
	SLObjectVersion * pNextVersion = m_TentativeVersion->m_pNextVersion;
	assert(pNextVersion);
	pNextVersion->m_validUpperTime = commitTime - 1;
	itemsSet.AddItem(m_pNVMAddress, m_pUserInfo->m_objectSize, m_TentativeVersion->m_pUserObject);

	if (m_openMode & OPEN_NEW)
	{
		unsigned int * pObjectReferenceCount = NVMMalloc::GetReferenceCountAddress(m_pNVMAddress);
		assert(pObjectReferenceCount);
		uint32_t rfcnt = 1;
		itemsSet.AddItem(pObjectReferenceCount, rfcnt);
	}
	else
	{
		TryCleanOldVersion();
	}
}

void CLTransactionalObject::WriteClose(CLWriteTransaction * pOwner)
{
	CLWriteTransaction * pNewOwner = nullptr;
	if (m_openMode & OPEN_DELETE)
	{
		pNewOwner = OBJECT_OWNER_POISION;
	}
	bool ret = m_pOwner.compare_exchange_strong(pOwner, pNewOwner);
	assert(ret == true);
	m_openMode &= OPEN_CLEAR_MASK;
	CLTransactionalObjectLookupTable::GetInstance().Put(m_pNVMAddress);
}

void CLTransactionalObject::WriteAbort(CLWriteTransaction * pOwner)
{
	if (m_openMode & OPEN_NEW)
	{
		ReleaseObject(this);
		return;
	}
	
	SLObjectVersion * pCurrent = m_TentativeVersion;
	m_TentativeVersion= pCurrent->m_pNextVersion;
	CLGarbageCollector::GetInstance().CollectGarbage(pCurrent, ReleaseVersion);

	bool ret = m_pOwner.compare_exchange_strong(pOwner, nullptr);
	assert(ret == true);
	m_openMode &= OPEN_CLEAR_MASK;
	CLTransactionalObjectLookupTable::GetInstance().Put(m_pNVMAddress);
}

bool CLTransactionalObject::ConvertReadToWrite(CLWriteTransaction * pOwner)
{
	assert(pOwner);
	if (pOwner != m_pOwner)
	{
		return false;
	}
	m_TentativeVersion = CloneANewVersion(m_TentativeVersion);
	m_openMode &= OPEN_WRITE;
	return true;
}

SLObjectVersion * CLTransactionalObject::ReadForReadTransaction(CLSnapShot & snapShot, CLReadTransactionReadedObjects & readSet)
{
	SLObjectVersion * pTmpVersion = FindNewestValidVersion();
	if (pTmpVersion->m_commitTime > snapShot.GetUpper())
	{
		snapShot.ExtendUpper(readSet.GetMinValidUpper());
	}

	do
	{
		if (pTmpVersion->m_commitTime <= snapShot.GetUpper())
		{
			snapShot.TrySetLower(pTmpVersion->m_commitTime);
			snapShot.TrySetUpper(pTmpVersion->m_validUpperTime);
			break;
		}
		else
		{
			pTmpVersion = pTmpVersion->m_pNextVersion;
		}
	} while (pTmpVersion);

	if (pTmpVersion && snapShot.IsValid())
	{
		return pTmpVersion;
	}
	return nullptr;
}

bool CLTransactionalObject::TryOccupyObject(CLTransactionalObject * pObject, CLWriteTransaction * pOwner)
{
	CLWriteTransaction * pObjectOwner = pObject->m_pOwner.load();
	if (pObjectOwner == pOwner)
	{
		return true;
	}
	else if (pObjectOwner != nullptr)
	{
		return false;
	}
	CLWriteTransaction * pExpected = nullptr;
	return pObject->m_pOwner.compare_exchange_strong(pExpected, pOwner);
}

void CLTransactionalObject::ReleaseVersion(void * pVersion)
{
	assert(pVersion);
	delete[](char *)(((SLObjectVersion *)pVersion)->m_pUserObject);
	delete (SLObjectVersion *)pVersion;
}

void CLTransactionalObject::TryCleanOldVersion()
{
	SLObjectVersion * pTmpVersion = m_TentativeVersion;
	SLObjectVersion * pTmpNextVersion = m_TentativeVersion->m_pNextVersion;
	int i = 0;
	while (pTmpNextVersion != nullptr)
	{
		++i;
		if (i > TRANSACTIONAL_OBJECT_MAX_CACHE_VERSION_COUNT)
		{
			break;
		}
		pTmpVersion = pTmpNextVersion;
		pTmpNextVersion = pTmpNextVersion->m_pNextVersion;
	}
	if (pTmpNextVersion != nullptr)
	{
		pTmpVersion->m_pNextVersion = nullptr;
		CLGarbageCollector::GetInstance().CollectGarbage(pTmpNextVersion, ReleaseVersion);
	}
}

SLObjectVersion * CLTransactionalObject::CloneANewVersion(SLObjectVersion * pVersion)
{
	assert(pVersion);
	void * ObjectCopy = new char[m_pUserInfo->m_objectSize];
	memcpy(ObjectCopy, pVersion->m_pUserObject, m_pUserInfo->m_objectSize);
	SLObjectVersion * pNewVersion = new SLObjectVersion(ObjectCopy, LSA_TIME_STAMP_INFINITE, pVersion,this);
	return pNewVersion;
}

SLObjectVersion * CLTransactionalObject::MakeANewVersion(void * pUserObject)
{
	assert(pUserObject);
	void * ObjectCopy = new char[m_pUserInfo->m_objectSize];
	memcpy(ObjectCopy, pUserObject, m_pUserInfo->m_objectSize);
	SLObjectVersion * pNewVersion = new SLObjectVersion(ObjectCopy, LSA_TIME_STAMP_START, nullptr,this);
	return pNewVersion;
}

TRANSACTIONLIB_NS_END