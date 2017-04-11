#ifndef __READ_WRITE_POINTER_H__
#define __READ_WRITE_POINTER_H__

#include "CLTransactionalObject.h"

template<typename T>
class CLReadWritePointer
{
public:
	CLReadWritePointer(void * pUserObject);
	~CLReadWritePointer();

public:
	const T * operator->();

public:
	inline bool IsValid();

private:
	CLTransactionalObject * m_pObject;
	void * m_pUserObject;
};

template<typename T>
inline CLReadWritePointer<T>::CLReadWritePointer(void * pUserObject):
m_pUserObject(pUserObject),
m_pObject(nullptr)
{
}

template<typename T>
inline CLReadWritePointer<T>::~CLReadWritePointer()
{
}

template<typename T>
inline const T * CLReadWritePointer<T>::operator->()
{
	return static_cast<T *>(m_pObject->GetUserObjectCopy());
}

template<typename T>
inline bool CLReadWritePointer<T>::IsValid()
{
	return m_pUserObject;
}

#endif