#ifndef __READ_ONLY_POINTER_H__
#define __READ_ONLY_POINTER_H__

#include "CLReadedObject.h"
#include "CLThreadTransactionManager.h"
#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

template<typename T>
class CLReadOnlyPointer
{
public:
	CLReadOnlyPointer(void * pUserObject);
	~CLReadOnlyPointer();

public:
	const T * operator->();
	const CLReadOnlyPointer<T> & operator=(void * pUserObject);
	const CLReadOnlyPointer<T> & operator=(const CLReadOnlyPointer &);
	bool operator==(const CLReadOnlyPointer &);

public:
	inline bool IsValid();

private:
	CLReadedObject * m_readedVersion;
};

template<typename T>
CLReadOnlyPointer<T>::CLReadOnlyPointer(void * pUserObject):
m_readedVersion(nullptr)
{
	if (pUserObject != nullptr)
	{
		m_readedVersion = CLThreadTransactionManager::GetInstance().GetReadTransaction().OpenObject(pUserObject,T::GetUserObjectInfo());
	}
}

template<typename T>
inline CLReadOnlyPointer<T>::~CLReadOnlyPointer()
{
}

template<typename T>
inline const T * CLReadOnlyPointer<T>::operator->()
{
	return static_cast<T *>(m_readedVersion->GetUserObjectCopy());
}

template<typename T>
const CLReadOnlyPointer<T> & CLReadOnlyPointer<T>::operator=(void * pUserObject)
{
	if (pUserObject == nullptr)
	{
		m_readedVersion = nullptr;
	}
	m_readedVersion = CLThreadTransactionManager::GetInstance().GetReadTransaction().OpenObject(pUserObject, T::GetUserObjectInfo());
	return *this;
}

template<typename T>
const CLReadOnlyPointer<T> & CLReadOnlyPointer<T>::operator=(const CLReadOnlyPointer & anotherPointer)
{
	m_readedVersion = anotherPointer.m_readedVersion;
}

template<typename T>
inline bool CLReadOnlyPointer<T>::operator==(const CLReadOnlyPointer & anotherPointer)
{
	return m_readedVersion == anotherPointer.m_readedVersion;
}

template<typename T>
bool CLReadOnlyPointer<T>::IsValid()
{
	return !!m_readedVersion;
}

TRANSACTIONLIB_NS_END
#endif