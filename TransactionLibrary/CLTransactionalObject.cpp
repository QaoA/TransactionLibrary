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
	//对于读事务，单纯减少引用计数
	//对于写事务，如果是new出来的，那么需要将nvm上数据的引用计数+1，如果是delete，那么需要将引用计数-1，如果减完后是0，那么还需要将该NVM对象归还
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

void * CLTransactionalObject::TryOpenForWriteTransaction(CLTransaction & writeTransaction)
{
	CLLocator * oldLocator = m_locator.load();
	CLLocator * newLocator = oldLocator->OpenForWriteTransaction(writeTransaction);
	if (newLocator == nullptr)
	{
		return nullptr;
	}
	if (newLocator == oldLocator)
	{
		return newLocator->GetTentativeUserObject();
	}
	bool isSuccess = m_locator.compare_exchange_weak(oldLocator, newLocator);
	if (!isSuccess)
	{
		newLocator->DoCloneInvalidCleanOperation();
		return nullptr;
	}
	else
	{
		CLGarbageCollector::GetInstance().CollectGarbage(oldLocator, CLLocator::ReleaseOldLocator);
	}
	return newLocator->GetTentativeUserObject();
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