#include "CLTransactionalObject.h"
#include "CLLocator.h"
#include "CLGarbageCollector.h"

CLTransactionalObject::CLTransactionalObject(CLTransaction & ownerTransaction, void * pNVMUserObject, SLUserObjectInfo * pUserObjectInfo) :
m_locator(new CLLocator(ownerTransaction,pNVMUserObject,pUserObjectInfo))
{
}

CLTransactionalObject::~CLTransactionalObject()
{
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
		CLGarbageCollector::GetInstance().CollectGarbage(oldLocator, CLLocator::DoRelease);
	}
}