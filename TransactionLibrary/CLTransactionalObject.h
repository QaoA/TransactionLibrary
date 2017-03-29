#ifndef __TRANSACTIONAL_OBJECT_H__
#define __TRANSACTIONAL_OBJECT_H__

#include "ObjectOpenMode.h"
#include "AtomicOperation.h"

class CLLocator;
struct SLUserObjectInfo;
class CLTransaction;
class CLReadedObject;
class CLSnapShot;
class CLReadTransactionReadedObjects;

class CLTransactionalObject
{
public:
	CLTransactionalObject(CLTransaction & ownerTransaction, void * pNVMUserObject, SLUserObjectInfo * pUserObjectInfo);
	~CLTransactionalObject();

public:
	bool TryOpenForWriteTransaction(CLTransaction & writeTransaction);
	inline CLReadedObject * ReadForReadTransaction(CLSnapShot & snapShot, CLReadTransactionReadedObjects & readSet);

private:
	CLLocator * m_locator;
};

inline CLReadedObject * CLTransactionalObject::ReadForReadTransaction(CLSnapShot & snapShot, CLReadTransactionReadedObjects & readSet)
{
	return m_locator->ReadForReadTransaction(*this,snapShot,readSet);
}

#endif