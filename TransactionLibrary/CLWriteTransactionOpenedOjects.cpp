#include "CLWriteTransactionOpenedOjects.h"
#include "CLTransactionalObject.h"
#include <cassert>

TRANSACTIONLIB_NS_BEGIN

using namespace std;

CLWriteTransactionOpenedOjects::CLWriteTransactionOpenedOjects()
{
}

CLWriteTransactionOpenedOjects::~CLWriteTransactionOpenedOjects()
{
}

CLTransactionalObject * CLWriteTransactionOpenedOjects::Find(void * pUserObjectNVMAddress)
{
	assert(pUserObjectNVMAddress);
	map<void *, CLTransactionalObject *>::iterator it = m_objectSet.find(pUserObjectNVMAddress);
	if (it == m_objectSet.end())
	{
		return nullptr;
	}
	return it->second;
}

void CLWriteTransactionOpenedOjects::AddObject(CLTransactionalObject * pObject)
{
	assert(pObject);
	m_objectSet.insert(make_pair(pObject->GetUserObjectNVMAddress(), pObject));
}

void CLWriteTransactionOpenedOjects::Commit(CLWriteTransaction * pOwner, CLLogItemsSet & itemSet, LSATimeStamp commitTime)
{
	assert(pOwner);
	for (map<void *, CLTransactionalObject *>::iterator it = m_objectSet.begin(); it != m_objectSet.end();)
	{
		CLTransactionalObject * pObject = it->second;
		if (pObject->GetOpenMode() & OPEN_WRITE)
		{
			pObject->WriteCommit(itemSet, commitTime);
			it++;
		}
		else if (pObject->GetOpenMode() & OPEN_READ)
		{
			pObject->ReadCommit(pOwner);
			m_objectSet.erase(it++);
		}
		assert(true);
	}
}

void CLWriteTransactionOpenedOjects::Abort(CLWriteTransaction * pOwner)
{
	assert(pOwner);
	for (map<void *, CLTransactionalObject *>::iterator it = m_objectSet.begin(); it != m_objectSet.end();)
	{
		CLTransactionalObject * pObject = it->second;
		if (pObject->GetOpenMode() & OPEN_WRITE)
		{
			pObject->WriteAbort(pOwner);
		}
		else if (pObject->GetOpenMode() & OPEN_READ)
		{
			pObject->ReadAbort(pOwner);
		}
		m_objectSet.erase(it++);
	}
}

void CLWriteTransactionOpenedOjects::Close(CLWriteTransaction * pOwner)
{
	assert(pOwner);
	for (map<void *, CLTransactionalObject *>::iterator it = m_objectSet.begin(); it != m_objectSet.end();)
	{
		CLTransactionalObject * pObject = it->second;
		assert(pObject->GetOpenMode() & OPEN_WRITE);
		pObject->WriteClose(pOwner);
		m_objectSet.erase(it++);
	}
}

void CLWriteTransactionOpenedOjects::Reset()
{
	assert(m_objectSet.empty());
}

TRANSACTIONLIB_NS_END