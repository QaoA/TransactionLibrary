#ifndef __READ_ONLY_POINTER_H__
#define __READ_ONLY_POINTER_H__

#include "CLReadedObject.h"
#include "CLThreadTransactionManager.h"

template<typename T>
class CLReadOnlyPointer
{
public:
	CLReadOnlyPointer(void * pUserObject);
	~CLReadOnlyPointer();

public:
	const T * operator->();

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
	return static_cast<T *>(m_readedVersion->GetUserObject());
}

template<typename T>
bool CLReadOnlyPointer<T>::IsValid()
{
	return !!m_readedVersion;
}

#endif