#ifndef __WRITE_POINTER_H__
#define __WRITE_POINTER_H__

#include "TransactionLibraryNameSpace.h"
#include "CLReadWritePointerImplementator.h"
#include "CLWritePointerImplementator.h"
#include "CLReadPointer.h"
#include <cassert>

TRANSACTIONLIB_NS_BEGIN

template<typename T>
class CLWritePointer
{
public:
	CLWritePointer();
	CLWritePointer(T * pNVMObject);
	CLWritePointer<T> & operator=(T * pNVMObject);
	CLWritePointer<T> & operator=(CLReadPointer<T> &);
	operator T *();
	~CLWritePointer();

private:
	CLWritePointer(T * pNVMObject, bool newFlag);

public:
	static CLWritePointer MakeNewPointer(T * pNVMObject);
	static void DeleteByPointer(CLWritePointer & writePointer);
	
public:
	inline T * operator->();
	inline T * Get();

private:
	CLWritePointerImplementator m_writePointer;
};

template<typename T>
inline CLWritePointer<T>::CLWritePointer()
{
}

template<typename T>
inline CLWritePointer<T>::CLWritePointer(T * pNVMObject)
{
	m_writePointer.Set(pNVMObject, T::GetUserObjectInfo());
}

template<typename T>
inline CLWritePointer<T>& CLWritePointer<T>::operator=(T * pNVMObject)
{
	m_writePointer.Set(pNVMObject, T::GetUserObjectInfo());
}

template<typename T>
inline CLWritePointer<T>& CLWritePointer<T>::operator=(CLReadPointer<T> & readPointer)
{
	m_writePointer.Set(readPointer.m_readWritePointer);
}

template<typename T>
inline CLWritePointer<T>::operator T*()
{
	return Get();
}

template<typename T>
inline CLWritePointer<T>::~CLWritePointer()
{
}

template<typename T>
inline CLWritePointer<T>::CLWritePointer(T * pNVMObject, bool newFlag)
{
	m_writePointer.Set(pNVMObject, T::GetUserObjectInfo(),true);
}

template<typename T>
inline CLWritePointer<T> CLWritePointer<T>::MakeNewPointer(T * pNVMObject)
{
	return CLWritePointer<T>(pNVMObject,true);
}

template<typename T>
inline void CLWritePointer<T>::DeleteByPointer(CLWritePointer & writePointer)
{
	writePointer.m_writePointer.SetRelease();
}

template<typename T>
inline T * CLWritePointer<T>::operator->()
{
	return (T *)m_writePointer.operator ->();
}

template<typename T>
inline T * CLWritePointer<T>::Get()
{
	return (T *)m_writePointer.Get();
}

TRANSACTIONLIB_NS_END

#endif