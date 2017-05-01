#include "CLWriteTransaction.h"
#include "CLTransactionalObject.h"
#include "CLTransactionAbort.h"
#include "NVMMalloc.h"
#include "CLLSAClock.h"
#include "CLThreadTransactionManager.h"
#include "CLGarbageCollector.h"
#include "CLLogArea.h"
#include "CLNameServer.h"
#include "CLNameAddressPair.h"
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
	m_nameAddressPairSet.Reset();
}

void CLWriteTransaction::Uninitialize()
{
	m_itemSet.ReleaseAllItems();
	m_objectSet.Close(this);
	m_nameAddressPairSet.Close();
	CLGarbageCollector::GetInstance().NotifyTransactionEnd();
}

void CLWriteTransaction::OnCommit()
{
	LSATimeStamp commitTime = CLLSAClock::GetInstance().Tick();
	m_objectSet.Commit(this, m_itemSet, commitTime);
	m_nameAddressPairSet.Commit(m_itemSet);
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
	m_nameAddressPairSet.Abort();
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

CLTransactionalObject * CLWriteTransaction::ConvertOpenModeReadToWrite(CLTransactionalObject * pObject)
{
	assert(pObject);
	if (!pObject->ConvertReadToWrite(this))
	{
		throw CLTransactionAbort(UNEXPECTED_ERROR);
	}
	return pObject;
}

bool CLWriteTransaction::SetAddressByName(char * strName, void * address)
{
	assert(strName);
	CLNameAddressPair * pPair =  m_nameAddressPairSet.FindByName(strName);
	if (pPair != nullptr)
	{
		pPair->Set(strName, address);
		return true;
	}
	
	SLNVMNameAddressPair * pNVMPair =  CLNameServer::GetInstance().FindAndGet(strName);
	if (pNVMPair == nullptr)
	{
		pNVMPair = CLNameServer::GetInstance().Get();
		if (pNVMPair == nullptr)
		{
			return false;
		}
	}
	pPair = new CLNameAddressPair(pNVMPair);
	m_nameAddressPairSet.AddObject(pPair);
	pPair->Set(strName, address);
	return true;
}

void * CLWriteTransaction::GetAddressByName(char * strName)
{
	assert(strName);
	CLNameAddressPair * pPair =  m_nameAddressPairSet.FindByName(strName);
	if (pPair)
	{
		return pPair;
	}
	
	SLNVMNameAddressPair * pNVMPair =  CLNameServer::GetInstance().Find(strName);
	if (pNVMPair == nullptr)
	{
		return nullptr;
	}
	pPair = new CLNameAddressPair(pNVMPair);
	m_nameAddressPairSet.AddObject(pPair);
	return pPair;
}

TRANSACTIONLIB_NS_END