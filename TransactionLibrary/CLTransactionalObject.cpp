#include "CLTransactionalObject.h"
#include "CLLocator.h"
#include "CLGarbageCollector.h"
#include "CLTransactionalObjectLookupTable.h"
#include "CLReadTransactionReadedObjects.h"
#include <cassert>

CLTransactionalObject::CLTransactionalObject(CLTransaction * ownerTransaction, void * pNVMUserObject, SLUserObjectInfo * pUserObjectInfo) :
m_locator(new CLLocator(ownerTransaction,pNVMUserObject,pUserObjectInfo)),
m_referenceCount(0)
{
}

CLTransactionalObject::~CLTransactionalObject()
{
}

CLTransactionalObject * CLTransactionalObject::MakeATransactionalObject(void * constructorArgs)
{
	assert(constructorArgs);
	SLTransactionalObjectCreatArgs * args = static_cast<SLTransactionalObjectCreatArgs *>(constructorArgs);
	return new CLTransactionalObject(args->m_ownerWriteTransaction,args->m_pNVMUserObject,args->m_pUserObjectInfo);
}

void CLTransactionalObject::ReleaseATransactionalObject(CLTransactionalObject * pObject)
{

}

CLTransactionalObject * CLTransactionalObject::OpenATransactionalObject(void * pNVMUserObject, CLTransaction * pOwnerTransaction, SLUserObjectInfo * pUserObjectInfo)
{
	assert(pNVMUserObject && pUserObjectInfo);
	SLTransactionalObjectCreatArgs args = { pOwnerTransaction,pNVMUserObject,pUserObjectInfo };
	CLTransactionalObject * pNewObject = CLTransactionalObjectLookupTable::GetInstance().FindOrCreate(pNVMUserObject,&args);
	pNewObject->IncreaseReferenceCount();
	return pNewObject;
}

void CLTransactionalObject::CloseATransactionalObject(CLTransactionalObject * pObject)
{
	assert(pObject);
	if (pObject->DecreaseReferenceCount() == 0)
	{
		CLGarbageCollector::GetInstance().CollectGarbage(pObject, (DestructFunc)CLTransactionalObject::ReleaseATransactionalObject);
	}
}

bool CLTransactionalObject::TryOpenForWriteTransaction(CLTransaction & writeTransaction)
{
	CLLocator * newLocator = m_locator.load()->TryCloneLocator(writeTransaction);
	if (newLocator == nullptr)
	{
		return false;
	}
	CLLocator * oldLocator = m_locator;
	bool isSuccess = m_locator.compare_exchange_weak(oldLocator, newLocator);
	if (!isSuccess)
	{
		newLocator->DoCloneInvalidClearOperation();
	}
	else
	{
		CLGarbageCollector::GetInstance().CollectGarbage(oldLocator, CLLocator::ReleaseOldLocator);
	}
}

CLReadedObject * CLTransactionalObject::ReadForReadTransaction(CLSnapShot & snapShot, CLReadTransactionReadedObjects & readSet)
{
	CLReadedObject * pReturnObject = m_locator.load()->ReadForReadTransaction(*this, snapShot, readSet);
	if (pReturnObject != nullptr)
	{
		readSet.AppendObject(pReturnObject);
	}
	return pReturnObject;
}