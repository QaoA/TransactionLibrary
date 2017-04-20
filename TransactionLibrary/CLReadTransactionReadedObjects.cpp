#include "CLReadTransactionReadedObjects.h"
#include "CLTransactionalObject.h"
#include "SLObjectVersion.h"
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

void CLReadTransactionReadedObjects::AppendObject(SLObjectVersion * pVersion)
{
	assert(pVersion);
	m_readSet.insert(make_pair(pVersion->m_pOwnerObject->GetUserObjectNVMAddress(), pVersion));
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
	LSATimeStamp minValidUpper = (m_readSet.begin()->second)->m_validUpperTime;
	for (pair<void * ,SLObjectVersion *> it : m_readSet)
	{
		SLObjectVersion * pVersion= it.second;
		if(pVersion->m_validUpperTime < minValidUpper)
		{
			minValidUpper = pVersion->m_validUpperTime;
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
	for (map<void *, SLObjectVersion *>::iterator it = m_readSet.begin(); it != m_readSet.end();)
	{
		SLObjectVersion * pVersion = it->second;
		pVersion->m_pOwnerObject->ReadOnlyCommit();
		m_readSet.erase(it++);
	}
}

void CLReadTransactionReadedObjects::Abort()
{
	for (map<void *, SLObjectVersion *>::iterator it = m_readSet.begin(); it != m_readSet.end();)
	{
		SLObjectVersion * pVersion = it->second;
		pVersion->m_pOwnerObject->ReadOnlyAbort();
		m_readSet.erase(it++);
	}
}

SLObjectVersion * CLReadTransactionReadedObjects::FindObject(void * pUserObject)
{
	assert(pUserObject);
	map<void *, SLObjectVersion *>::iterator it = m_readSet.find(pUserObject);
	if (it == m_readSet.end())
	{
		return nullptr;
	}
	return it->second;
}

TRANSACTIONLIB_NS_END