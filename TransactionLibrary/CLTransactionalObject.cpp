#include "CLTransactionalObject.h"
#include "CLTransactionalObjectLookupTable.h"
#include "SLUserObjectInfo.h"
#include "CLGarbageCollector.h"
#include "CLLogItemsSet.h"
#include "CLSnapShot.h"
#include "CLReadTransactionReadedObjects.h"
#include "CLReadedObject.h"
#include <cstring>
#include <cassert>

CLTransactionalObject::SLVersion::SLVersion(void * pUserObject, LSATimeStamp timestamp, SLVersion * nextVersion):
m_pUserObject(pUserObject),
m_commitTime(timestamp),
m_pNextVersion(nextVersion)
{
}

CLTransactionalObject::CLTransactionalObject(SLTransactionalObjectCreatArgs & args):
m_pOwner(args.m_owner),
m_pUserInfo(args.m_pUserObjectInfo),
m_pNVMAddress(args.m_pNVMUserObject)
{
	switch (args.m_openMode)
	{
	case READ_ONLY:
	case READ:
		m_TentativeVersion = MakeANewVersion(args.m_pNVMUserObject);
		break;
	case WRITE:
		SLVersion * pReadVersion = MakeANewVersion(args.m_pNVMUserObject);
		m_TentativeVersion = CloneANewVersion(pReadVersion);
		break;
	default:
		break;
	}
}

CLTransactionalObject * CLTransactionalObject::MakeObject(void * pArgs)
{
	assert(pArgs);
	return new CLTransactionalObject(*(SLTransactionalObjectCreatArgs *)(pArgs));
}

void CLTransactionalObject::ReleaseObject(CLTransactionalObject * pObject)
{
	assert(pObject);
	SLVersion * pCurrentVersion = pObject->m_TentativeVersion;
	while (pCurrentVersion)
	{
		SLVersion * pTmpVersion = pCurrentVersion;
		pCurrentVersion = pCurrentVersion->m_pNextVersion;
		ReleaseVersion(pTmpVersion);
	}
}

CLTransactionalObject * CLTransactionalObject::ReadOnlyOpen(void * pNVMUserObject, SLUserObjectInfo * pUserObjectInfo)
{
	assert(pNVMUserObject && pUserObjectInfo);
	SLTransactionalObjectCreatArgs args = { nullptr,pNVMUserObject,pUserObjectInfo,READ_ONLY };
	return CLTransactionalObjectLookupTable::GetInstance().GetOrCreate(pNVMUserObject, &args);
}

void CLTransactionalObject::ReadOnlyClose()
{
	CLTransactionalObjectLookupTable::GetInstance().Put(m_pNVMAddress);
}

void CLTransactionalObject::ReadOnlyAbort()
{
	CLTransactionalObjectLookupTable::GetInstance().Put(m_pNVMAddress);
}

CLTransactionalObject * CLTransactionalObject::ReadWriteOpen(void * pNVMUserObject, CLWriteTransaction * pOwner, SLUserObjectInfo * pUserObjectInfo)
{
	assert(pNVMUserObject && pOwner && pUserObjectInfo);
	SLTransactionalObjectCreatArgs args = { pOwner,pNVMUserObject,pUserObjectInfo,READ };
	CLTransactionalObject * pObject = CLTransactionalObjectLookupTable::GetInstance().GetOrCreate(pNVMUserObject, &args);
	assert(pObject);
	bool ret = TryOccupyObject(pObject,pOwner);
	if (ret)
	{
		return pObject;
	}
	CLTransactionalObjectLookupTable::GetInstance().Put(pObject->m_pNVMAddress);
	return nullptr;
}

void CLTransactionalObject::ReadWriteClose(CLWriteTransaction * pOwner)
{
	assert(pOwner);
	bool ret = m_pOwner.compare_exchange_strong(pOwner, nullptr);
	assert(ret == true);
	CLTransactionalObjectLookupTable::GetInstance().Put(m_pNVMAddress);
}

void CLTransactionalObject::ReadWriteAbort(CLWriteTransaction * pOwner)
{
	assert(pOwner);
	bool ret = m_pOwner.compare_exchange_strong(pOwner, nullptr);
	assert(ret == true);
	CLTransactionalObjectLookupTable::GetInstance().Put(m_pNVMAddress);
}

CLTransactionalObject * CLTransactionalObject::WriteOpen(void * pNVMUserObject, CLWriteTransaction * pOwner, SLUserObjectInfo * pUserObjectInfo)
{
	assert(pNVMUserObject, pOwner, pUserObjectInfo);
	SLTransactionalObjectCreatArgs args = { pOwner,pNVMUserObject,pUserObjectInfo,WRITE };
	CLTransactionalObject * pObject = CLTransactionalObjectLookupTable::GetInstance().GetOrCreate(pNVMUserObject, &args);
	assert(pObject);
	bool ret = TryOccupyObject(pObject, pOwner);
	if (ret)
	{
		SLVersion * pOldVersion = pObject->m_TentativeVersion;
		pObject->m_TentativeVersion = pObject->CloneANewVersion(pOldVersion);
		return pObject;
	}
	CLTransactionalObjectLookupTable::GetInstance().Put(pObject->m_pNVMAddress);
	return nullptr;
}

void CLTransactionalObject::WriteCommit(CLLogItemsSet & itemsSet, LSATimeStamp commitTime)
{
	m_TentativeVersion->m_commitTime = commitTime;
	itemsSet.AddItem(m_pNVMAddress, m_pUserInfo->m_objectSize, m_TentativeVersion->m_pUserObject);
	TryCleanOldVersion();
}

void CLTransactionalObject::WriteClose(CLWriteTransaction * pOwner)
{
	bool ret = m_pOwner.compare_exchange_strong(pOwner, nullptr);
	assert(ret == true);
	CLTransactionalObjectLookupTable::GetInstance().Put(m_pNVMAddress);
}

void CLTransactionalObject::WriteAbort(CLWriteTransaction * pOwner)
{
	SLVersion * pCurrent = m_TentativeVersion;
	m_TentativeVersion= pCurrent->m_pNextVersion;
	bool ret = m_pOwner.compare_exchange_strong(pOwner, nullptr);
	assert(ret == true);
	CLGarbageCollector::GetInstance().CollectGarbage(pCurrent, ReleaseVersion);
	CLTransactionalObjectLookupTable::GetInstance().Put(m_pNVMAddress);
}

CLReadedObject * CLTransactionalObject::ReadForReadTransaction(CLSnapShot & snapShot, CLReadTransactionReadedObjects & readSet)
{
	SLVersion * pFirstVersion = FindNewestValidVersion();
	if (pFirstVersion->m_commitTime > snapShot.GetUpper())
	{
		snapShot.ExtendUpper(readSet.GetMinValidUpper());
	}

	SLVersion * readedVersion = nullptr;
	LSATimeStamp tmpUpper = LSA_TIME_STAMP_INFINITE;
	for (SLVersion * tmpVersion = pFirstVersion; tmpVersion != nullptr; tmpVersion = tmpVersion->m_pNextVersion)
	{
		if (tmpVersion->m_commitTime <= snapShot.GetUpper())
		{
			snapShot.TrySetLower(tmpVersion->m_commitTime);
			snapShot.TrySetUpper(tmpUpper);
			readedVersion = tmpVersion;
			break;
		}
		else
		{
			tmpUpper = tmpVersion->m_commitTime;
		}
	}
	if (readedVersion && snapShot.IsValid())
	{
		return new CLReadedObject(this, readedVersion->m_pUserObject, readedVersion->m_commitTime, tmpUpper);
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
	delete[](char *)(((SLVersion *)pVersion)->m_pUserObject);
	delete (SLVersion *)pVersion;
}

void CLTransactionalObject::TryCleanOldVersion()
{
	SLVersion * pTmpVersion = m_TentativeVersion;
	SLVersion * pTmpNextVersion = m_TentativeVersion->m_pNextVersion;
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

CLTransactionalObject::SLVersion * CLTransactionalObject::CloneANewVersion(SLVersion * pVersion)
{
	assert(pVersion);
	void * ObjectCopy = new char[m_pUserInfo->m_objectSize];
	memcpy(ObjectCopy, pVersion->m_pUserObject, m_pUserInfo->m_objectSize);
	SLVersion * pNewVersion = new SLVersion(ObjectCopy, LSA_TIME_STAMP_INFINITE, pVersion);
	return pNewVersion;
}

CLTransactionalObject::SLVersion * CLTransactionalObject::MakeANewVersion(void * pUserObject)
{
	assert(pUserObject);
	void * ObjectCopy = new char[m_pUserInfo->m_objectSize];
	memcpy(ObjectCopy, pUserObject, m_pUserInfo->m_objectSize);
	SLVersion * pNewVersion = new SLVersion(ObjectCopy, LSA_TIME_STAMP_START, nullptr);
	return pNewVersion;
}