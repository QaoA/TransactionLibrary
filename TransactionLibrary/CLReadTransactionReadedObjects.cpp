#include "CLReadTransactionReadedObjects.h"
#include "CLReadedObject.h"
#include "CLTransactionalObject.h"
#include <cassert>

TRANSACTIONLIB_NS_BEGIN

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
	m_readSet.insert(make_pair(pObject->GetUserObjectNVMAddress(),pObject));
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
	LSATimeStamp minValidUpper = (m_readSet.begin()->second)->GetUpperTime();
	for (pair<void * ,CLReadedObject *> it : m_readSet)
	{
		CLReadedObject * pObject = it.second;
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
	for (map<void *, CLReadedObject *>::iterator it = m_readSet.begin(); it != m_readSet.end();)
	{
		CLReadedObject * pObject = it->second;
		pObject->GetTransactionalObject()->ReadOnlyCommit();
		delete pObject;
		m_readSet.erase(it++);
	}
}

void CLReadTransactionReadedObjects::Abort()
{
	for (map<void *, CLReadedObject *>::iterator it = m_readSet.begin(); it != m_readSet.end();)
	{
		CLReadedObject * pObject = it->second;
		pObject->GetTransactionalObject()->ReadOnlyAbort();
		delete pObject;
		m_readSet.erase(it++);
	}
}

CLReadedObject * CLReadTransactionReadedObjects::FindObject(void * pUserObject)
{
	assert(pUserObject);
	map<void *, CLReadedObject *>::iterator it = m_readSet.find(pUserObject);
	if (it == m_readSet.end())
	{
		return nullptr;
	}
	return it->second;
}

bool CLReadTransactionReadedObjects::CompareReadedObject(CLReadedObject * pObject1, CLReadedObject * pObject2)
{
	assert(pObject1 && pObject2);
}

TRANSACTIONLIB_NS_END