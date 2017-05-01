#include "CLReadWritePointerImplementator.h"
#include "CLThreadTransactionManager.h"
#include "SLUserObjectInfo.h"
#include "CLTransactionalObject.h"
#include "CLTransactionAbort.h"

TRANSACTIONLIB_NS_BEGIN
	
CLReadWritePointerImplementator::CLReadWritePointerImplementator():
m_pObject(nullptr)
{
}

CLReadWritePointerImplementator::~CLReadWritePointerImplementator()
{
}

void CLReadWritePointerImplementator::Set(void * pNVMUserObject, SLUserObjectInfo * pUserObjectInfo)
{
	if (pNVMUserObject != nullptr)
	{
		m_pObject = CLThreadTransactionManager::GetWriteTransaction()->OpenObjectRead(pNVMUserObject, pUserObjectInfo);
		if (m_pObject == nullptr)
		{
			throw CLTransactionAbort(OBJECT_OCCUPIED_BY_ANOTHER_WRITE_TRANSACTION);
		}
		return;
	}
	m_pObject = nullptr;
}

void CLReadWritePointerImplementator::Set(CLReadWritePointerImplementator & anotherOne)
{
	m_pObject = anotherOne.m_pObject;
}

void * CLReadWritePointerImplementator::operator->()
{
	return m_pObject->GetTentativeVersionUserObjectCopy();
}

void * CLReadWritePointerImplementator::Get()
{
	return m_pObject->GetUserObjectNVMAddress();
}

TRANSACTIONLIB_NS_END