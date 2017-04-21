#include "CLReadTransaction.h"
#include "CLTransactionalObject.h"
#include "CLTransactionAbort.h"
#include "CLThreadTransactionManager.h"
#include <cassert>

TRANSACTIONLIB_NS_BEGIN

CLReadTransaction::CLReadTransaction()
{
}

CLReadTransaction::~CLReadTransaction()
{
}

void CLReadTransaction::Initialize()
{
	m_snapShot.Reset();
	m_readSet.Reset();
}

void CLReadTransaction::Uninitialize()
{
}

void CLReadTransaction::OnCommit()
{
	m_readSet.Commit();
}

void CLReadTransaction::OnAbort(CLTransactionAbort &)
{
	m_readSet.Abort();
}

SLObjectVersion * CLReadTransaction::OpenObject(void * pUserObject, SLUserObjectInfo * pUserObjectInfo)
{
	assert(pUserObjectInfo);

	SLObjectVersion * pReadedVersion = m_readSet.FindObject(pUserObject);
	if (pReadedVersion)
	{
		return pReadedVersion;
	}

	CLTransactionalObject * pObject = CLTransactionalObject::ReadOnlyOpen(pUserObject, pUserObjectInfo);
	if (pObject == nullptr)
	{
		throw CLTransactionAbort(UNEXPECTED_ERROR);
	}
	pReadedVersion = pObject->ReadForReadTransaction(m_snapShot, m_readSet);
	if (pReadedVersion == nullptr)
	{
		pObject->ReadOnlyAbort();
		throw CLTransactionAbort(NO_VALID_VERSION_IN_READ_TRANSACION);
	}	
	m_readSet.AppendObject(pReadedVersion);
	return pReadedVersion;
}

TRANSACTIONLIB_NS_END