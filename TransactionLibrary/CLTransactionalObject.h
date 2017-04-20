#ifndef __TRANSACTIONAL_OBJECT_H__
#define __TRANSACTIONAL_OBJECT_H__

#include <atomic>
#include "LSATimeStamp.h"
#include "SLObjectVersion.h"
#include "EMObjectOpenMode.h"
#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

#define TRANSACTIONAL_OBJECT_MAX_CACHE_VERSION_COUNT 3

class CLWriteTransaction;
struct SLUserObjectInfo;
class CLLogItemsSet;
class CLSnapShot;
class CLReadTransactionReadedObjects;

class CLTransactionalObject
{
private:
	struct SLTransactionalObjectCreatArgs
	{
		CLWriteTransaction * m_owner;
		void * m_pNVMUserObject;
		SLUserObjectInfo * m_pUserObjectInfo;
		EMObjectOpenMode m_openMode;
	};

private:
	CLTransactionalObject(SLTransactionalObjectCreatArgs & args);

public:
	static CLTransactionalObject * MakeObject(void * pArgs);
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
	inline EMObjectOpenMode GetOpenMode();

private:
	static inline bool TryOccupyObject(CLTransactionalObject * pObject, CLWriteTransaction * pOwner);
	static void ReleaseVersion(void * pVersion);

private:
	void TryCleanOldVersion();
	inline SLObjectVersion * FindNewestValidVersion();

private:
	SLObjectVersion * CloneANewVersion(SLObjectVersion * pVersion);
	SLObjectVersion * MakeANewVersion(void * pUserObject);

private:
	std::atomic<CLWriteTransaction *> m_pOwner;
	SLObjectVersion * m_TentativeVersion;
	SLUserObjectInfo  * m_pUserInfo;
	void * m_pNVMAddress;
	EMObjectOpenMode m_openMode;
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
	m_openMode &= OPEN_READ;
}

inline void CLTransactionalObject::MarkWrite()
{
	m_openMode &= OPEN_WRITE;
}

inline EMObjectOpenMode CLTransactionalObject::GetOpenMode()
{
	return m_openMode;
}

TRANSACTIONLIB_NS_END
#endif