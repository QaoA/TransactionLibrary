#include "CLWriteTransaction.h"
#include "CLTransactionalObject.h"
#include "CLTransactionAbort.h"
#include "NVMMalloc.h"
#include "CLLSAClock.h"
#include "CLThreadTransactionManager.h"
#include "CLGarbageCollector.h"
#include "CLLogArea.h"
#include "CLNameServer.h"
#include "SLNVMNameAddressPair.h"
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
	CLGarbageCollector::GetInstance().NotifyTransactionBegin();
	m_objectSet.Reset();
	m_itemSet.Reset();
}

void CLWriteTransaction::Uninitialize()
{
	m_itemSet.ReleaseAllItems();
	m_objectSet.Close(this);
	CLGarbageCollector::GetInstance().NotifyTransactionEnd();
}

void CLWriteTransaction::OnCommit()
{
	LSATimeStamp commitTime = CLLSAClock::GetInstance().Tick();
	m_objectSet.Commit(this, m_itemSet, commitTime);
	NVMMalloc::CLLogArea * logArea = NVMMalloc::AllocLogArea();
	assert(logArea);
	logArea->WriteLogStart();
	m_itemSet.WriteLogs(*logArea);
	logArea->WriteLogEnd();
	m_itemSet.SetValues();
	NVMMalloc::FreeLogArea(logArea);
}

void CLWriteTransaction::OnAbort(CLTransactionAbort &)
{
	m_objectSet.Abort(this);
}

CLTransactionalObject * CLWriteTransaction::OpenObjectRead(void * pUserObject, SLUserObjectInfo * pUserObjectInfo)
{
	assert(pUserObject);
	CLTransactionalObject * pObject = m_objectSet.Find(pUserObject);
	if (pObject)
	{
		goto markReadAndReturn;
	}
	pObject = CLTransactionalObject::ReadOpen(pUserObject, this, pUserObjectInfo);
	if (pObject == nullptr)
	{
		throw CLTransactionAbort(OBJECT_OCCUPIED_BY_ANOTHER_WRITE_TRANSACTION);
	}
	m_objectSet.AddObject(pObject);

markReadAndReturn:
	pObject->MarkRead();
	return pObject;
}

CLTransactionalObject * CLWriteTransaction::OpenObjectWrite(void * pUserObject, SLUserObjectInfo * pUserObjectInfo)
{
	assert(pUserObject);
	CLTransactionalObject * pObject = m_objectSet.Find(pUserObject);
	if (pObject)
	{
		goto markWriteAndReturn;
	}
	pObject = CLTransactionalObject::WriteOpen(pUserObject, this, pUserObjectInfo);
	if (pObject == nullptr)
	{
		throw CLTransactionAbort(OBJECT_OCCUPIED_BY_ANOTHER_WRITE_TRANSACTION);
	}
	m_objectSet.AddObject(pObject);

markWriteAndReturn:
	pObject->MarkWrite();
	return pObject;
}

void CLWriteTransaction::ConvertOpenModeReadToWrite(CLTransactionalObject * pObject)
{
	assert(pObject);
	if (!pObject->ConvertReadToWrite(this))
	{
		throw CLTransactionAbort(UNEXPECTED_ERROR);
	}
}

bool CLWriteTransaction::SetName(char * strName, void * address)
{
	assert(strName);
	
}

SLNVMNameAddressPair * CLWriteTransaction::FindNameAddressPairByName(char * strName)
{
	assert(strName);
}

void * CLWriteTransaction::FindByName(char * strName)
{
	return nullptr;
}

TRANSACTIONLIB_NS_END