#ifndef __READ_WRITE_POINTER_H__
#define __READ_WRITE_POINTER_H__

#include "CLTransactionalObject.h"
#include "CLWriteTransaction.h"
#include "TransactionLibraryNameSpace.h"
#include <cassert>

TRANSACTIONLIB_NS_BEGIN

template<typename T>
class CLReadWritePointer
{
public:
	CLReadWritePointer(T * pUserObject);
	~CLReadWritePointer();

public:
	inline const T * operator->();
	inline bool IsValid();

private:
	CLTransactionalObject * m_pObject;
};

template<typename T>
inline CLReadWritePointer<T>::CLReadWritePointer(T * pUserObject):
m_pObject(nullptr)
{
	if (pUserObject != nullptr)
	{
		m_pObject = CLThreadTransactionManager::GetInstance().GetWriteTransaction().OpenObjectRead(pUserObject, T::GetUserObjectInfo());
	}
}

template<typename T>
inline CLReadWritePointer<T>::~CLReadWritePointer()
{
}

template<typename T>
inline const T * CLReadWritePointer<T>::operator->()
{
	assert(m_pObject);
	return static_cast<T *>(m_pObject->GetTentativeVersionUserObjectCopy());
}

template<typename T>
inline bool CLReadWritePointer<T>::IsValid()
{
	return !!m_pObject;
}

TRANSACTIONLIB_NS_END
#endif