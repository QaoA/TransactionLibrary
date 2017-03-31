#ifndef __TRANSACTIONAL_OBJECT_PROXY_H__
#define __TRANSACTIONAL_OBJECT_PROXY_H__

#include "LSATimeStamp.h"
#include "SLUserObjectInfo.h"
#include <cstring>

class CLSnapShot;
class CLTransaction;
class CLTransactionalObject;
class CLReadedObject;
class CLReadTransactionReadedObjects;

class CLLocator
{
private:
	struct SLVersion
	{
		SLVersion(LSATimeStamp timestamp, void * pObject, SLVersion * pNextVersion);
		LSATimeStamp m_timeStamp;
		void * m_pObject;
		SLVersion * m_pNextVersion;
	};

public:
	CLLocator(CLTransaction * ownerTransaction,void * pNVMUserObject, SLUserObjectInfo * pUserObjectInfo);
	~CLLocator();

public:
	CLReadedObject * ReadForReadTransaction(CLTransactionalObject & ownerTransactionalObject, CLSnapShot & snapShot, CLReadTransactionReadedObjects & readSet);
	CLLocator * TryCloneLocator(CLTransaction & ownerTransaction);
	void DoCloneInvalidClearOperation();

public:
	static void ReleaseOldLocator(void * locator);

private:
	inline SLVersion * MakeVersion(void * pNVMUserObject, size_t userObjectSize);
	inline SLVersion * CloneVersion(SLVersion * pVersion, size_t userObjectSize);
	inline void * CloneUserObject(const void * pObject, size_t objectSize);
	SLVersion * FindNewestValidVersion();
	inline void PrepareDataAfterClone(CLTransaction & ownerTransaction);

private:
	CLTransaction * m_owner;
	void * m_pNVMUserObject;
	SLUserObjectInfo * m_pUserObjectInfo;
	SLVersion * m_TentativeVersion;
};

inline CLLocator::SLVersion * CLLocator::MakeVersion(void * pNVMUserObject, size_t userObjectSize)
{
	return new SLVersion(LSA_TIME_STAMP_START, CloneUserObject(pNVMUserObject, userObjectSize), nullptr);
}

inline CLLocator::SLVersion * CLLocator::CloneVersion(SLVersion * pVersion, size_t userObjectSize)
{
	return new SLVersion(LSA_TIME_STAMP_INFINITE, CloneUserObject(pVersion->m_pObject, userObjectSize), pVersion);
}

inline void * CLLocator::CloneUserObject(const void * pObject, size_t objectSize)
{
	void * pReturnObject = new char[objectSize];
	memcpy(pReturnObject, pObject, objectSize);
	return pReturnObject;
}

inline void CLLocator::PrepareDataAfterClone(CLTransaction & ownerTransaction)
{
	m_owner = &ownerTransaction;
	m_TentativeVersion = CloneVersion(m_TentativeVersion, m_pUserObjectInfo->m_objectSize);
}

#endif