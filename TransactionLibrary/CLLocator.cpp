#include "CLLocator.h"
#include "CLBasicData.h"
#include "CLLSAClock.h"
#include "CLTransaction.h"
#include "CLReadedObject.h"
#include "CLSnapShot.h"
#include "CLReadTransactionReadedObjects.h"
#include <cassert>
#include <cstring>
#include <cstdlib>

CLLocator::SLVersion::SLVersion(LSATimeStamp timestamp, void * pObject, SLVersion * pNextVersion):
m_timeStamp(timestamp),
m_pObject(pObject),
m_pNextVersion(pNextVersion)
{
}

CLLocator::CLLocator(CLTransaction * pOwnerTransaction, void * pNVMUserObject, SLUserObjectInfo * pUserObjectInfo):
m_owner(pOwnerTransaction),
m_pNVMUserObject(pNVMUserObject),
m_pUserObjectInfo(pUserObjectInfo),
m_TentativeVersion(new SLVersion(LSA_TIME_STAMP_START,CloneUserObject(pNVMUserObject,pUserObjectInfo->m_objectSize),nullptr))
{
	assert(pNVMUserObject && pUserObjectInfo);
}

CLLocator::~CLLocator()
{
}

CLLocator::CLLocator(CLLocator & anotherLocator, CLTransaction & ownerWriteTransaction):
m_owner(&ownerWriteTransaction),
m_pNVMUserObject(anotherLocator.m_pNVMUserObject),
m_pUserObjectInfo(anotherLocator.m_pUserObjectInfo),
m_TentativeVersion(new SLVersion(LSA_TIME_STAMP_INFINITE,CloneUserObject(anotherLocator.m_TentativeVersion->m_pObject,anotherLocator.m_pUserObjectInfo->m_objectSize),anotherLocator.m_TentativeVersion))
{
}

CLReadedObject * CLLocator::ReadForReadTransaction(CLTransactionalObject & ownerTransactionalObject, CLSnapShot & snapShot, CLReadTransactionReadedObjects & readSet)
{
	SLVersion * pFirstVersion = FindNewestValidVersion();
	if (pFirstVersion->m_timeStamp > snapShot.GetUpper())
	{
		snapShot.ExtendUpper(readSet.GetMinValidUpper());
	}

	SLVersion * readedVersion = nullptr;
	LSATimeStamp tmpUpper = LSA_TIME_STAMP_INFINITE;
	for (SLVersion * tmpVersion = pFirstVersion; tmpVersion != nullptr; tmpVersion = tmpVersion->m_pNextVersion)
	{
		if (tmpVersion->m_timeStamp <= snapShot.GetUpper())
		{
			snapShot.TrySetLower(tmpVersion->m_timeStamp);
			snapShot.TrySetUpper(tmpUpper);
			readedVersion = tmpVersion;
			break;
		}
		else
		{
			tmpUpper = tmpVersion->m_timeStamp;
		}
	}
	if (readedVersion && snapShot.IsValid())
	{
		return new CLReadedObject(&ownerTransactionalObject, readedVersion->m_pObject, readedVersion->m_timeStamp, tmpUpper);
	}
	return nullptr;
}

CLLocator * CLLocator::OpenForWriteTransaction(CLTransaction & ownerTransaction)
{
	if (m_owner == &ownerTransaction)
	{
		return this;
	}
	if (m_owner && m_owner->GetStatus() == ACTIVE)
	{
		return nullptr;
	}
	return new CLLocator(*this, ownerTransaction);
}

void CLLocator::DoCloneInvalidCleanOperation()
{
	assert(m_owner&&m_TentativeVersion&&m_TentativeVersion->m_pNextVersion == nullptr);
	delete[](char *)m_TentativeVersion->m_pObject;
	delete m_TentativeVersion;
	delete this;
}

void CLLocator::ReleaseOldLocator(void * locator)
{
	delete static_cast<CLLocator *>(locator);
}

CLLocator::SLVersion * CLLocator::FindNewestValidVersion()
{
	if (m_owner == nullptr)
	{
		return m_TentativeVersion;
	}
	switch (m_owner->GetStatus())
	{
	case ACTIVE:
	case ABORTED:
		return m_TentativeVersion->m_pNextVersion;
		break;
	case COMMITTED:
		return m_TentativeVersion;
		break;
	default:
		break;
	}
}
