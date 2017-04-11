#ifndef __WRITE_POINTER_H__
#define __WRITE_POINTER_H__

#include "CLTransactionalObject.h"
#include "CLThreadTransactionManager.h"
#include "CLTransactionAbort.h"

template<typename T>
class CLWritePointer
{
public:
	CLWritePointer(T * pUserObject);
	~CLWritePointer();

public:
	inline T * operator->();

private:
	CLTransactionalObject * m_pTransactionalObject;
	void * m_pObject;
};

template<typename T>
CLWritePointer<T>::CLWritePointer(T * pUserObject) :
	m_pTransactionalObject(CLTransactionalObject::OpenATransactionalObject(pUserObject, CLThreadTransactionManager::GetWriteTransaction(), T::GetUserInfo())),
	m_pObject(nullptr)
{
	if (m_pTransactionalObject == nullptr)
	{
		throw CLTransactionAbort(UNEXPECTED_ERROR);
	}
	m_pObject = m_pTransactionalObject->TryOpenForWriteTransaction(CLThreadTransactionManager::GetWriteTransaction());
	if (m_pObject == nullptr)
	{
		throw CLTransactionAbort(OBJECT_OCCUPIED_BY_ANOTHER_WRITE_TRANSACTION);
	}
}

template<typename T>
CLWritePointer<T>::~CLWritePointer()
{
	CLTransactionalObject::CloseATransactionalObject(m_pTransactionalObject);
}

template<typename T>
inline T * CLWritePointer<T>::operator->()
{
	return m_pObject;
}

#endif