#ifndef __WRITE_POINTER_H__
#define __WRITE_POINTER_H__

#include "CLTransactionalObject.h"
#include "CLThreadTransactionManager.h"
#include "CLTransactionAbort.h"
#include "CLWriteTransaction.h"
#include "TransactionLibraryNameSpace.h"
#include "CLReadWritePointer.h"
#include <cassert>

TRANSACTIONLIB_NS_BEGIN

template<typename T>
class CLWritePointer
{
public:
	CLWritePointer(T * pUserObject);
	~CLWritePointer();

public:
	CLWritePointer<T> & operator=(T * pUserObject);
	CLWritePointer<T> & operator=(const CLWritePointer &);
	CLWritePointer<T> & operator=(const CLReadWritePointer<T> &);
	bool operator==(const CLWritePointer &);

public:
	inline T * operator->();
	inline bool IsValid();

private:
	CLTransactionalObject * m_pObject;
};

template<typename T>
CLWritePointer<T>::CLWritePointer(T * pUserObject) :
m_pObject(nullptr)
{
	if (pUserObject != nullptr)
	{
		m_pObject = CLThreadTransactionManager::GetWriteTransaction()->OpenObjectWrite(pUserObject, T::GetUserObjectInfo());
	}
}

template<typename T>
CLWritePointer<T>::~CLWritePointer()
{
}

template<typename T>
CLWritePointer<T>& CLWritePointer<T>::operator=(T * pUserObject)
{
	if (pUserObject != nullptr)
	{
		m_pObject = CLThreadTransactionManager::GetWriteTransaction()->OpenObjectWrite(pUserObject, T::GetUserObjectInfo());
	}
	else
	{
		m_pObject = nullptr;
	}
	return *this;
}

template<typename T>
CLWritePointer<T>& CLWritePointer<T>::operator=(const CLWritePointer & anotherPointer)
{
	m_pObject = anotherPointer.m_pObject;
}

template<typename T>
CLWritePointer<T>& CLWritePointer<T>::operator=(const CLReadWritePointer<T> & anotherPointer)
{
	m_pObject = CLThreadTransactionManager::GetWriteTransaction()->ConvertOpenModeReadToWrite(anotherPointer.m_pObject);
}

template<typename T>
inline bool CLWritePointer<T>::operator==(const CLWritePointer & anotherPointer)
{
	return m_pObject == anotherPointer.m_pObject;
}

template<typename T>
inline T * CLWritePointer<T>::operator->()
{
	assert(m_pObject);
	return static_cast<T *>(m_pObject->GetTentativeVersionUserObjectCopy());
}

template<typename T>
inline bool CLWritePointer<T>::IsValid()
{
	return !!m_pObject;
}

TRANSACTIONLIB_NS_END

#endif