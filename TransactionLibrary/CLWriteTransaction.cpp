#include "CLWriteTransaction.h"
#include "CLTransactionalObject.h"
#include "CLTransactionAbort.h"
#include "NVMMalloc.h"
#include "CLBasicData.h"
#include "CLLSAClock.h"
#include "CLLogArea.h"
#include <cassert>

TRANSACTIONLIB_NS_BEGIN

CLWriteTransaction::CLWriteTransaction()
{
}

CLWriteTransaction::~CLWriteTransaction()
{
}

void CLWriteTransaction::Initialize()
{
	m_readSet.Reset();
	m_writeSet.Reset();
	m_itemSet.Reset();
}

void CLWriteTransaction::Uninitialize()
{
	m_writeSet.CloseAll(this);
	m_itemSet.ReleaseAllItems();
}

void CLWriteTransaction::OnCommit()
{
	LSATimeStamp commitTime = CLBasicData::GetInstance().GetLSAClock().Tick();
	m_writeSet.Commit(m_itemSet, commitTime);

	CLLogArea * logArea = NVMMalloc::AllocLogArea();
	assert(logArea);
	logArea->SetLogAreaDataInvalid();
	m_itemSet.WriteLogs(*logArea);
	logArea->SetLogAreaDataValid();
	m_itemSet.SetValues();
	NVMMalloc::FreeLogArea(logArea);

	m_readSet.Commit(this);
}

void CLWriteTransaction::OnAbort(CLTransactionAbort &)
{
	m_writeSet.Abort(this);
	m_readSet.Abort(this);
}

CLTransactionalObject * CLWriteTransaction::OpenObjectRead(void * pUserObject, SLUserObjectInfo * pUserObjectInfo)
{
	assert(pUserObject);
	CLTransactionalObject * pObject = CLTransactionalObject::ReadOpen(pUserObject, this, pUserObjectInfo);
	if (pObject == nullptr)
	{
		throw CLTransactionAbort(OBJECT_OCCUPIED_BY_ANOTHER_WRITE_TRANSACTION);
	}
	m_readSet.AddObject(pObject);
	return pObject;
}

CLTransactionalObject * CLWriteTransaction::OpenObjectWrite(void * pUserObject, SLUserObjectInfo * pUserObjectInfo)
{
	assert(pUserObject);
	CLTransactionalObject * pObject = CLTransactionalObject::WriteOpen(pUserObject, this, pUserObjectInfo);
	if (pObject == nullptr)
	{
		throw CLTransactionAbort(OBJECT_OCCUPIED_BY_ANOTHER_WRITE_TRANSACTION);
	}
	m_writeSet.AddObject(pObject);
	return pObject;
}

void CLWriteTransaction::ConvertOpenModeReadToWrite(CLTransactionalObject * pObject)
{
	assert(pObject);
	if (m_readSet.RemoveObject(pObject))
	{
		if (!pObject->ConvertReadToWrite(this))
		{
			throw CLTransactionAbort(UNEXPECTED_ERROR);
		}
		m_writeSet.AddObject(pObject);
	}
}

TRANSACTIONLIB_NS_END