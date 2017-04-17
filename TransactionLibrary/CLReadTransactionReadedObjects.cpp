#include "CLReadTransactionReadedObjects.h"
#include "CLReadedObject.h"
#include "CLTransactionalObject.h"
#include <cassert>

using namespace std;

CLReadTransactionReadedObjects::CLReadTransactionReadedObjects():
m_readSet(),
m_bIsClosed(false),
m_minValidUpper(LSA_TIME_STAMP_INVALID)
{
}

CLReadTransactionReadedObjects::~CLReadTransactionReadedObjects()
{
}

void CLReadTransactionReadedObjects::Reset()
{
	assert(m_readSet.empty());
	m_bIsClosed = true;
	m_minValidUpper = LSA_TIME_STAMP_INVALID;
}

void CLReadTransactionReadedObjects::AppendObject(CLReadedObject * pObject)
{
	assert(pObject);
	m_readSet.push_back(pObject);
}

LSATimeStamp CLReadTransactionReadedObjects::GetMinValidUpper()
{
	if (m_bIsClosed)
	{
		return m_minValidUpper;
	}

	if (m_readSet.empty())
	{
		return LSA_TIME_STAMP_INFINITE;
	}
	LSATimeStamp minValidUpper = (*m_readSet.begin())->GetUpperTime();
	for (CLReadedObject * pObject : m_readSet)
	{
		if (pObject->GetUpperTime() < minValidUpper)
		{
			minValidUpper = pObject->GetUpperTime();
		}
	}

	if (minValidUpper != LSA_TIME_STAMP_INFINITE)
	{
		m_bIsClosed = true;
		m_minValidUpper = minValidUpper;
	}
}

void CLReadTransactionReadedObjects::Commit()
{
	for (std::list<CLReadedObject *>::iterator it = m_readSet.begin(); it != m_readSet.end();)
	{
		(*it)->GetTransactionalObject()->ReadOnlyCommit();
		delete (*it);
		it = m_readSet.erase(it);
	}
}

void CLReadTransactionReadedObjects::Abort()
{
	for (std::list<CLReadedObject *>::iterator it = m_readSet.begin(); it != m_readSet.end();)
	{
		(*it)->GetTransactionalObject()->ReadOnlyAbort();
		delete (*it);
		it = m_readSet.erase(it);
	}
}