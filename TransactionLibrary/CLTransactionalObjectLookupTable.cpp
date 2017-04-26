#include "CLTransactionalObjectLookupTable.h"
#include "CLTransactionalObject.h"
#include "CLCriticalSection.h"
#include <cassert>

TRANSACTIONLIB_NS_BEGIN

CLTransactionalObjectLookupTable::CLTransactionalObjectLookupTable()
{
}

CLTransactionalObjectLookupTable::~CLTransactionalObjectLookupTable()
{
}

CLTransactionalObjectLookupTable & CLTransactionalObjectLookupTable::GetInstance()
{
	static CLTransactionalObjectLookupTable instance;
	return instance;
}

CLTransactionalObject * CLTransactionalObjectLookupTable::GetOrCreate(void * pNVMObject, SLTransactionalObjectCreatArgs * pArgs)
{
	CLCriticalSection cs(&m_lock);
	CLTransactionalObject * pObject = m_objectTree.FindOrCreate(pNVMObject, pArgs);
	pObject->IncreaseReferenceCount();
	return pObject;
}

void CLTransactionalObjectLookupTable::Put(CLTransactionalObject * pObject, bool CanDelete)
{
	CLCriticalSection cs(&m_lock);
	if (pObject->DecreaseReferenceCount() == 0)
	{
		if (CanDelete == true)
		{
			m_objectTree.GetAndDelete(pObject->GetUserObjectNVMAddress());
		}
		else
		{
			m_objectTree.GetAndRemove(pObject->GetUserObjectNVMAddress());
		}
	}
}

TRANSACTIONLIB_NS_END