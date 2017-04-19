#include "CLWriteTransactionWriteObjects.h"
#include "CLTransactionalObject.h"

TRANSACTIONLIB_NS_BEGIN

using namespace std;

CLWriteTransactionWriteObjects::CLWriteTransactionWriteObjects()
{
}

CLWriteTransactionWriteObjects::~CLWriteTransactionWriteObjects()
{
}

void CLWriteTransactionWriteObjects::Reset()
{
	assert(m_writeSet.empty());
}

void CLWriteTransactionWriteObjects::AddObject(CLTransactionalObject * pObject)
{
	assert(pObject);
	m_writeSet.insert(pObject);
}

void CLWriteTransactionWriteObjects::Commit(CLLogItemsSet & itemSet, LSATimeStamp commitTime)
{
	for (CLTransactionalObject * pObject : m_writeSet)
	{
		pObject->WriteCommit(itemSet, commitTime);
	}
}

void CLWriteTransactionWriteObjects::Abort(CLWriteTransaction * pOwner)
{
	for (set<CLTransactionalObject *>::iterator it = m_writeSet.begin(); it != m_writeSet.end(); )
	{
		(*it)->WriteAbort(pOwner);
	}
}

void CLWriteTransactionWriteObjects::CloseAll(CLWriteTransaction * pOwner)
{
	for (set<CLTransactionalObject *>::iterator it = m_writeSet.begin(); it != m_writeSet.end(); )
	{
		(*it)->WriteClose(pOwner);
		m_writeSet.erase(it++);
	}
}

TRANSACTIONLIB_NS_END