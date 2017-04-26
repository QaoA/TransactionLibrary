#ifndef __TRANSACTIONAL_OBJECT_H__
#define __TRANSACTIONAL_OBJECT_H__

#include <atomic>
#include "LSATimeStamp.h"
#include "SLObjectVersion.h"
#include "EMObjectOpenMode.h"
#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

#define TRANSACTIONAL_OBJECT_MAX_CACHE_VERSION_COUNT 3
#define OBJECT_OWNER_POISION (CLWriteTransaction *)(1)

class CLWriteTransaction;
struct SLUserObjectInfo;
class CLLogItemsSet;
class CLSnapShot;
class CLReadTransactionReadedObjects;
struct SLTransactionalObjectCreatArgs;

class CLTransactionalObject
{
private:
	CLTransactionalObject(SLTransactionalObjectCreatArgs & args, LSATimeStamp lastValidVersion);

public:
	static CLTransactionalObject * MakeObject(SLTransactionalObjectCreatArgs * pArgs, LSATimeStamp lastValidVersion);
	static void ReleaseObject(CLTransactionalObject * pObject);
	static CLTransactionalObject * ReadOnlyOpen(void * pNVMUserObject, SLUserObjectInfo * pUserObjectInfo);
	static CLTransactionalObject * ReadOpen(void * pNVMUserObject, CLWriteTransaction * pOwner, SLUserObjectInfo * pUserObjectInfo);
	static CLTransactionalObject * WriteOpen(void * pNVMUserObject, CLWriteTransaction * pOwner, SLUserObjectInfo * pUserObjectInfo);

public:
	void ReadOnlyCommit();
	void ReadOnlyAbort();
	void ReadCommit(CLWriteTransaction * pOwner);
	void ReadAbort(CLWriteTransaction * pOwner);
	void WriteCommit(CLLogItemsSet & itemsSet, LSATimeStamp commitTime);
	void WriteClose(CLWriteTransaction * pOwner);
	void WriteAbort(CLWriteTransaction * pOwner);
	bool ConvertReadToWrite(CLWriteTransaction * pOwner);
	SLObjectVersion * ReadForReadTransaction(CLSnapShot & snapShot, CLReadTransactionReadedObjects & readSet);

public:
	inline void * GetTentativeVersionUserObjectCopy();
	inline void * GetUserObjectNVMAddress();
	inline void MarkRead();
	inline void MarkWrite();
	inline void MarkNew();
	inline void MarkDelete();
	inline EMObjectOpenMode GetOpenMode();
	inline unsigned int IncreaseReferenceCount();
	inline unsigned int DecreaseReferenceCount();
	inline LSATimeStamp GetLastCommitTimeStamp();

private:
	static inline bool TryOccupyObject(CLTransactionalObject * pObject, CLWriteTransaction * pOwner);
	static void ReleaseVersion(void * pVersion);

private:
	void TryCleanOldVersion();
	inline SLObjectVersion * FindNewestValidVersion();

private:
	SLObjectVersion * CloneANewVersion(SLObjectVersion * pVersion);
	SLObjectVersion * MakeANewVersion(void * pUserObject, LSATimeStamp lastValidVersion);

private:
	std::atomic<CLWriteTransaction *> m_pOwner;
	SLObjectVersion * m_TentativeVersion;
	SLUserObjectInfo  * m_pUserInfo;
	void * m_pNVMAddress;
	EMObjectOpenMode m_openMode;
	unsigned int m_objectReferenceCount;
};

inline SLObjectVersion * CLTransactionalObject::FindNewestValidVersion()
{
	SLObjectVersion * tentativeVersion = m_TentativeVersion;
	if (tentativeVersion->m_commitTime == LSA_TIME_STAMP_INFINITE)
	{
		return tentativeVersion->m_pNextVersion;
	}
	return tentativeVersion;
}

inline void * CLTransactionalObject::GetTentativeVersionUserObjectCopy()
{
	return m_TentativeVersion->m_pUserObject;
}

inline void * CLTransactionalObject::GetUserObjectNVMAddress()
{
	return m_pNVMAddress;
}

inline void CLTransactionalObject::MarkRead()
{
	m_openMode |= OPEN_READ;
}

inline void CLTransactionalObject::MarkWrite()
{
	m_openMode |= OPEN_WRITE;
}

inline void CLTransactionalObject::MarkNew()
{
	m_openMode |= OPEN_NEW;
}

inline void CLTransactionalObject::MarkDelete()
{
	m_openMode |= OPEN_DELETE;
}

inline EMObjectOpenMode CLTransactionalObject::GetOpenMode()
{
	return m_openMode;
}

inline unsigned int CLTransactionalObject::IncreaseReferenceCount()
{
	return ++m_objectReferenceCount;
}

inline unsigned int CLTransactionalObject::DecreaseReferenceCount()
{
	return --m_objectReferenceCount;
}

inline LSATimeStamp CLTransactionalObject::GetLastCommitTimeStamp()
{
	LSATimeStamp commitTime = m_TentativeVersion->m_commitTime;
	if (commitTime != LSA_TIME_STAMP_INFINITE)
	{
		return commitTime;
	}
	return m_TentativeVersion->m_pNextVersion->m_commitTime;
}

TRANSACTIONLIB_NS_END
#endif