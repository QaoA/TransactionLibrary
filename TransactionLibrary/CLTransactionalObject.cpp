#include "CLTransactionalObject.h"
#include "CLLocator.h"
#include "CLGarbageCollector.h"
#include <cassert>

CLTransactionalObject::CLTransactionalObject(CLTransaction & ownerTransaction, void * pNVMUserObject, SLUserObjectInfo * pUserObjectInfo) :
m_locator(new CLLocator(ownerTransaction,pNVMUserObject,pUserObjectInfo))
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

bool CLTransactionalObject::TryOpenForWriteTransaction(CLTransaction & writeTransaction)
{
	CLLocator * newLocator = m_locator->TryCloneLocator(writeTransaction);
	if (newLocator == nullptr)
	{
		return false;
	}
	CLLocator * oldLocator = m_locator;
	bool isSuccess = BoolCompareAndSet(&m_locator, oldLocator, newLocator);
	if (!isSuccess)
	{
		newLocator->DoCloneInvalidClearOperation();
	}
	else
	{
		CLGarbageCollector::GetInstance().CollectGarbage(oldLocator, CLLocator::ReleaseOldLocator);
	}
}