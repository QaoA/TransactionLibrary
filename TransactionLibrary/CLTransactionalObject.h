#ifndef __TRANSACTIONAL_OBJECT_H__
#define __TRANSACTIONAL_OBJECT_H__

#include <atomic>
#include "LSATimeStamp.h"

#define TRANSACTIONAL_OBJECT_MAX_CACHE_VERSION_COUNT 3

class CLWriteTransaction;
struct SLUserObjectInfo;
class CLLogItemsSet;
class CLSnapShot;
class CLReadTransactionReadedObjects;
class CLReadedObject;

class CLTransactionalObject
{
private:
	enum EMObjectOpenMode
	{
		READ_ONLY = 0x1,
		READ = 0x2,
		WRITE = 0x4
	};
	struct SLVersion
	{
		SLVersion(void * pUserObject, LSATimeStamp timestamp, SLVersion * nextVersion);
		void * m_pUserObject;
		LSATimeStamp m_commitTime;
		SLVersion * m_pNextVersion;
	};
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
	static CLTransactionalObject * ReadWriteOpen(void * pNVMUserObject, CLWriteTransaction * pOwner, SLUserObjectInfo * pUserObjectInfo);
	static CLTransactionalObject * WriteOpen(void * pNVMUserObject, CLWriteTransaction * pOwner, SLUserObjectInfo * pUserObjectInfo);

public:
	void ReadOnlyClose();
	void ReadOnlyAbort();
	void ReadWriteClose(CLWriteTransaction * pOwner);
	void ReadWriteAbort(CLWriteTransaction * pOwner);
	void WriteCommit(CLLogItemsSet & itemsSet, LSATimeStamp commitTime);
	void WriteClose(CLWriteTransaction * pOwner);
	void WriteAbort(CLWriteTransaction * pOwner);
	CLReadedObject * ReadForReadTransaction(CLSnapShot & snapShot, CLReadTransactionReadedObjects & readSet);

public:
	inline void * GetUserObjectCopy();

private:
	static inline bool TryOccupyObject(CLTransactionalObject * pObject, CLWriteTransaction * pOwner);
	static void ReleaseVersion(void * pVersion);

private:
	void TryCleanOldVersion();
	inline SLVersion * FindNewestValidVersion();

private:
	SLVersion * CloneANewVersion(SLVersion * pVersion);
	SLVersion * MakeANewVersion(void * pUserObject);

private:
	std::atomic<CLWriteTransaction *> m_pOwner;
	SLVersion * m_TentativeVersion;
	SLUserObjectInfo  * m_pUserInfo;
	void * m_pNVMAddress;
};

inline CLTransactionalObject::SLVersion * CLTransactionalObject::FindNewestValidVersion()
{
	SLVersion * tentativeVersion = m_TentativeVersion;
	if (tentativeVersion->m_commitTime == LSA_TIME_STAMP_INFINITE)
	{
		return tentativeVersion->m_pNextVersion;
	}
	return tentativeVersion;
}

inline void * CLTransactionalObject::GetUserObjectCopy()
{
	return m_TentativeVersion->m_pUserObject;
}

#endif