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

CLTransactionalObject * CLTransactionalObjectLookupTable::GetOrCreate(void * pNVMObject, void * transactionalObjectCreateArgs)
{
	assert(pNVMObject);
	CLCriticalSection cs(&m_lock);
	SLTransactionalObjectWrapper * pWrapper = m_objectTree.Get(pNVMObject);
	if (pWrapper == nullptr)
	{
		pWrapper = new SLTransactionalObjectWrapper{ CLTransactionalObject::MakeObject(transactionalObjectCreateArgs),0 };;
		m_objectTree.Insert(pWrapper, pNVMObject);
	}
	pWrapper->m_referenceCount++;
	return pWrapper->m_pObject;
}

void CLTransactionalObjectLookupTable::Put(void * pNVMObject)
{
	assert(pNVMObject);
	CLCriticalSection cs(&m_lock);
	SLTransactionalObjectWrapper * pWrapper = m_objectTree.GetAndRemove(pNVMObject);
	assert(pWrapper != nullptr);
	if (pWrapper->m_referenceCount == 1)
	{
		CLTransactionalObject::ReleaseObject(pWrapper->m_pObject);
	}
	delete pWrapper;
}

TRANSACTIONLIB_NS_END