#ifndef __READ_ONLY_POINTER_H__
#define __READ_ONLY_POINTER_H__

#include "CLTransactionalObject.h"
#include "CLReadedObject.h"
#include "CLThreadTransactionManager.h"
#include "CLTransactionAbort.h"

template<typename T>
class CLReadOnlyPointer
{
public:
	CLReadOnlyPointer(T * pNVMUserObject);
	~CLReadOnlyPointer();

public:
	const T * operator->() const;

private:
	CLTransactionalObject * m_pTransactionalObject;
	CLReadedObject * m_readedObject;
};

template<typename T>
inline CLReadOnlyPointer<T>::CLReadOnlyPointer(T * pNVMUserObject):
m_pTransactionalObject(CLTransactionalObject::OpenATransactionalObject(pNVMUserObject, nullptr, T::GetUserObjectInfo())),
m_readedObject(nullptr)
{
}

template<typename T>
inline CLReadOnlyPointer<T>::~CLReadOnlyPointer()
{
	CLTransactionalObject::ReleaseATransactionalObject(m_pTransactionalObject);
}

template<typename T>
const T * CLReadOnlyPointer<T>::operator->() const
{
	if (m_readedObject == nullptr)
	{
		m_readedObject = m_pTransactionalObject->ReadForReadTransaction(CLThreadTransactionManager::GetReadTransaction().GetSnapShot(), 
			CLThreadTransactionManager::GetReadTransaction().GetReadSet());
		if (m_readedObject == nullptr)
		{
			throw CLTransactionAbort(NO_VALID_REASON_IN_READ_TRANSACION);
		}
	}
	return static_cast<T *>(m_readedObject->GetUserObject());
}

#endif