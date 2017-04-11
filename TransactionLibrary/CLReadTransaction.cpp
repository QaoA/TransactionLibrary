#include "CLReadTransaction.h"
#include "CLTransactionalObject.h"
#include "CLTransactionAbort.h"
#include <cassert>

CLReadTransaction::CLReadTransaction()
{
}

CLReadTransaction::~CLReadTransaction()
{
}

CLReadedObject * CLReadTransaction::OpenObject(void * pUserObject, SLUserObjectInfo * pUserObjectInfo)
{
	assert(pUserObjectInfo);
	CLTransactionalObject * pObject = CLTransactionalObject::ReadOnlyOpen(pUserObject, pUserObjectInfo);
	if (pObject == nullptr)
	{
		throw CLTransactionAbort(UNEXPECTED_ERROR);
	}
	CLReadedObject * pReadedObject = pObject->ReadForReadTransaction(m_snapShot, m_readSet);
	if (pReadedObject == nullptr)
	{
		pObject->ReadOnlyAbort();
		throw CLTransactionAbort(NO_VALID_VERSION_IN_READ_TRANSACION);
	}	
	m_readSet.AppendObject(pReadedObject);
	return pReadedObject;
}