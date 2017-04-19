#include "CLWriteTransactionReadObjects.h"
#include "CLTransactionalObject.h"

TRANSACTIONLIB_NS_BEGIN

using namespace std;

CLWriteTransactionReadObjects::CLWriteTransactionReadObjects()
{
}

CLWriteTransactionReadObjects::~CLWriteTransactionReadObjects()
{
}

void CLWriteTransactionReadObjects::Reset()
{
	assert(m_readSet.empty());
}

void CLWriteTransactionReadObjects::AddObject(CLTransactionalObject * pObject)
{
	assert(pObject);
	m_readSet.insert(pObject);
}

bool CLWriteTransactionReadObjects::RemoveObject(CLTransactionalObject * pObject)
{
	return !!m_readSet.erase(pObject);
}

void CLWriteTransactionReadObjects::Commit(CLWriteTransaction * pOwner)
{
	for (set<CLTransactionalObject *>::iterator it = m_readSet.begin(); it != m_readSet.end(); )
	{
		(*it)->ReadCommit(pOwner);
		m_readSet.erase(it++);
	}
}

void CLWriteTransactionReadObjects::Abort(CLWriteTransaction * pOwner)
{
	for (set<CLTransactionalObject *>::iterator it = m_readSet.begin(); it != m_readSet.end();)
	{
		(*it)->ReadAbort(pOwner);
		m_readSet.erase(it++);
	}
}

TRANSACTIONLIB_NS_END