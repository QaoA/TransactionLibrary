#include "CLWritePointerImplementator.h"
#include "CLThreadTransactionManager.h"
#include "CLTransactionalObject.h"
#include "CLReadWritePointerImplementator.h"
#include "CLWriteTransaction.h"
#include "CLTransactionAbort.h"

TRANSACTIONLIB_NS_BEGIN

CLWritePointerImplementator::CLWritePointerImplementator()
{
}

CLWritePointerImplementator::~CLWritePointerImplementator()
{
}

void CLWritePointerImplementator::Set(void * pNVMUserObject, SLUserObjectInfo * pUserObjectInfo, bool IsNew)
{
	if (pNVMUserObject != nullptr)
	{
		m_pObject = CLThreadTransactionManager::GetWriteTransaction()->OpenObjectWrite(pNVMUserObject, pUserObjectInfo);
		if (m_pObject == nullptr)
		{
			throw CLTransactionAbort(OBJECT_OCCUPIED_BY_ANOTHER_WRITE_TRANSACTION);
		}
	}
	if (IsNew)
	{
		m_pObject->MarkNew();
	}
}

void CLWritePointerImplementator::Set(CLReadWritePointerImplementator & readWritePointer)
{
	m_pObject = CLThreadTransactionManager::GetWriteTransaction()->ConvertOpenModeReadToWrite(readWritePointer.m_pObject);
}

void CLWritePointerImplementator::SetRelease()
{
	m_pObject->MarkDelete();
	m_pObject = nullptr;
}

void * CLWritePointerImplementator::Get()
{
	if (m_pObject)
	{
		return m_pObject->GetUserObjectNVMAddress();
	}
	return nullptr;
}

void * CLWritePointerImplementator::operator->()
{
	if (m_pObject)
	{
		return m_pObject->GetTentativeVersionUserObjectCopy();
	}
	return nullptr;
}
TRANSACTIONLIB_NS_END
