#include "CLWriteTransactionWriteObjects.h"
#include "CLTransactionalObject.h"

CLWriteTransactionWriteObjects::CLWriteTransactionWriteObjects()
{
}

CLWriteTransactionWriteObjects::~CLWriteTransactionWriteObjects()
{
}

void CLWriteTransactionWriteObjects::Commit(CLLogItemsSet & itemSet, LSATimeStamp commitTime)
{
	for (CLTransactionalObject * pObject : m_writeSet)
	{
		pObject->WriteCommit(itemSet, commitTime);
	}
}

void CLWriteTransactionWriteObjects::CloseAll(CLWriteTransaction * pOwner)
{
	for (CLTransactionalObject * pObject : m_writeSet)
	{
		pObject->WriteClose(pOwner);
	}
}

void CLWriteTransactionWriteObjects::Abort(CLWriteTransaction * pOwner)
{
	for (CLTransactionalObject * pObject : m_writeSet)
	{
		pObject->WriteAbort(pOwner);
	}
}