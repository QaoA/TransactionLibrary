#ifndef __TRANSACTIONAL_OBJECT_H__
#define __TRANSACTIONAL_OBJECT_H__

#include "ObjectOpenMode.h"
#include "CLLocator.h"
#include <atomic>

struct SLUserObjectInfo;
class CLTransaction;
class CLReadedObject;
class CLSnapShot;
class CLReadTransactionReadedObjects;

class CLTransactionalObject
{
private:
	struct SLTransactionalObjectCreatArgs
	{
		CLTransaction * m_ownerWriteTransaction;
		void * m_pNVMUserObject;
		SLUserObjectInfo * m_pUserObjectInfo;
	};

private:
	CLTransactionalObject(CLTransaction * ownerTransaction, void * pNVMUserObject, SLUserObjectInfo * pUserObjectInfo);

public:
	~CLTransactionalObject();

public:
	static CLTransactionalObject * MakeATransactionalObject(void * constructorArgs);
	static void ReleaseATransactionalObject(CLTransactionalObject * pObject);
	static CLTransactionalObject * OpenATransactionalObject(void * pNVMUserObject, CLTransaction * pOwnerTransaction, SLUserObjectInfo * pUserObjectInfo);
	static void CloseATransactionalObject(CLTransactionalObject * pObject);

public:
	void * TryOpenForWriteTransaction(CLTransaction & writeTransaction);
	CLReadedObject * ReadForReadTransaction(CLSnapShot & snapShot, CLReadTransactionReadedObjects & readSet);

public:
	inline unsigned int IncreaseReferenceCount();
	inline unsigned int DecreaseReferenceCount();

private:
	std::atomic<CLLocator *> m_locator;
	std::atomic<unsigned int> m_referenceCount;
};

inline unsigned int CLTransactionalObject::IncreaseReferenceCount()
{
	return ++m_referenceCount;
}

inline unsigned int CLTransactionalObject::DecreaseReferenceCount()
{
	return --m_referenceCount;
}

#endif