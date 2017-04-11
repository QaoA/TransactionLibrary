#include "CLWriteTransactionReadObjects.h"
#include "CLTransactionalObject.h"

CLWriteTransactionReadObjects::CLWriteTransactionReadObjects()
{
}

CLWriteTransactionReadObjects::~CLWriteTransactionReadObjects()
{
}

void CLWriteTransactionReadObjects::Commit(CLWriteTransaction * pOwner)
{
	for (CLTransactionalObject * pObject : m_readSet)
	{
		pObject->ReadClose(pOwner);
	}
	m_readSet.clear();
}

void CLWriteTransactionReadObjects::Abort(CLWriteTransaction * pOwner)
{
	for (CLTransactionalObject * pObject : m_readSet)
	{
		pObject->ReadAbort(pOwner);
	}
	m_readSet.clear();
}
