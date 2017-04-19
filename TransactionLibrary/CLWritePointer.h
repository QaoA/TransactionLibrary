#ifndef __WRITE_POINTER_H__
#define __WRITE_POINTER_H__

#include "CLTransactionalObject.h"
#include "CLThreadTransactionManager.h"
#include "CLTransactionAbort.h"
#include "CLWriteTransaction.h"
#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

template<typename T>
class CLWritePointer
{
public:
	CLWritePointer(T * pUserObject);
	~CLWritePointer();

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
		m_pObject = CLThreadTransactionManager::GetInstance().GetWriteTransaction().OpenObjectWrite(pUserObject, T::GetUserObjectInfo());
	}
}

template<typename T>
CLWritePointer<T>::~CLWritePointer()
{
}

template<typename T>
inline T * CLWritePointer<T>::operator->()
{
	assert(m_pObject);
	return m_pObject->GetTentativeVersionUserObjectCopy();
}

template<typename T>
inline bool CLWritePointer<T>::IsValid()
{
	return !!m_pObject;
}

TRANSACTIONLIB_NS_END

#endif