#include "CLReadOnlyPointerImplementator.h"
#include "SLObjectVersion.h"
#include "CLTransactionalObject.h"
#include "CLThreadTransactionManager.h"
#include "CLTransactionAbort.h"

TRANSACTIONLIB_NS_BEGIN

CLReadOnlyPointerImplementator::CLReadOnlyPointerImplementator():
m_readedVersion(nullptr)
{
}

CLReadOnlyPointerImplementator::~CLReadOnlyPointerImplementator()
{
}

void CLReadOnlyPointerImplementator::Set(void * pNVMUserObject, SLUserObjectInfo * pUserObjectInfo)
{
	if (pNVMUserObject != nullptr)
	{
		m_readedVersion = CLThreadTransactionManager::GetReadTransaction()->OpenObject(pNVMUserObject, pUserObjectInfo);
		if (m_readedVersion == nullptr)
		{
			throw CLTransactionAbort(NO_VALID_VERSION_IN_READ_TRANSACION);
		}
		return;
	}
	pNVMUserObject = nullptr;
}

void CLReadOnlyPointerImplementator::Set(CLReadOnlyPointerImplementator & anotherOne)
{
	m_readedVersion = anotherOne.m_readedVersion;
}

void * CLReadOnlyPointerImplementator::operator->()
{
	return m_readedVersion->m_pUserObject;
}

void * CLReadOnlyPointerImplementator::Get()
{
	return m_readedVersion->m_pOwnerObject->GetUserObjectNVMAddress();
}

TRANSACTIONLIB_NS_END