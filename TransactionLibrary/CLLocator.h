#ifndef __TRANSACTIONAL_OBJECT_PROXY_H__
#define __TRANSACTIONAL_OBJECT_PROXY_H__

#include "LSATimeStamp.h"
#include "SLUserObjectInfo.h"
#include <cstring>
#include <cassert>

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

private:
	CLLocator(CLLocator & anotherLocator, CLTransaction & ownerWriteTransaction);

public:
	CLReadedObject * ReadForReadTransaction(CLTransactionalObject & ownerTransactionalObject, CLSnapShot & snapShot, CLReadTransactionReadedObjects & readSet);
	CLLocator * OpenForWriteTransaction(CLTransaction & ownerTransaction);
	void DoCloneInvalidCleanOperation();
	inline void * GetTentativeUserObject();

public:
	static void ReleaseOldLocator(void * locator);

private:
	inline void * CloneUserObject(const void * pObject, size_t objectSize);
	SLVersion * FindNewestValidVersion();

private:
	CLTransaction * m_owner;
	void * m_pNVMUserObject;
	SLUserObjectInfo * m_pUserObjectInfo;
	SLVersion * m_TentativeVersion;
};

inline void * CLLocator::GetTentativeUserObject()
{
	assert(m_TentativeVersion && m_TentativeVersion->m_pObject);
	return m_TentativeVersion->m_pObject;
}

inline void * CLLocator::CloneUserObject(const void * pObject, size_t objectSize)
{
	void * pReturnObject = new char[objectSize];
	memcpy(pReturnObject, pObject, objectSize);
	return pReturnObject;
}

#endif