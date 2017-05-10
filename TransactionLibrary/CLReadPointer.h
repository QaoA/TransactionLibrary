#ifndef __READ_POINTER_H__
#define __READ_POINTER_H__

#include "TransactionLibraryNameSpace.h"
#include "CLReadOnlyPointerImplementator.h"
#include "CLReadWritePointerImplementator.h"
#include "CLTransactionAPIs.h"

TRANSACTIONLIB_NS_BEGIN

template<typename T>
class CLWritePointer;

template<typename T>
class CLReadPointer
{
	friend class CLWritePointer<T>;
public:
	CLReadPointer();
	CLReadPointer(T * pNVMObject);
	CLReadPointer(const CLReadPointer &);
	CLReadPointer<T> & operator=(T * pNVMObject);
	bool operator==(CLReadPointer &);
	bool operator!=(CLReadPointer &);
	~CLReadPointer();
	
public:
	inline T * operator->();
	inline T * Get();
	
private:
	union
	{
		CLReadOnlyPointerImplementator m_readOnlyPointer;
		CLReadWritePointerImplementator m_readWritePointer;
	};
};

template<typename T>
inline CLReadPointer<T>::CLReadPointer()
{
}

template<typename T>
CLReadPointer<T>::CLReadPointer(T * pNVMObject)
{
	if (CLTransactionAPIs::IsInReading())
	{
		m_readOnlyPointer.Set(pNVMObject, T::GetUserObjectInfo());
	}
	else
	{
		m_readWritePointer.Set(pNVMObject, T::GetUserObjectInfo());
	}
}

template<typename T>
CLReadPointer<T>::CLReadPointer(const CLReadPointer & anotherOne):
m_readOnlyPointer(anotherOne.m_readOnlyPointer)
{
}

template<typename T>
CLReadPointer<T> & CLReadPointer<T>::operator=(T * pNVMObject)
{
	if (CLTransactionAPIs::IsInReading())
	{
		m_readOnlyPointer.Set(pNVMObject, T::GetUserObjectInfo());
	}
	else
	{
		m_readWritePointer.Set(pNVMObject, T::GetUserObjectInfo());
	}
}

template<typename T>
inline bool CLReadPointer<T>::operator==(CLReadPointer & anotherOne)
{
	if (CLTransactionAPIs::IsInReading())
	{
		return anotherOne.m_readOnlyPointer.Get() == m_readOnlyPointer.Get();
	}
	else
	{
		return anotherOne.m_readWritePointer.Get() == m_readWritePointer.Get();
	}
}

template<typename T>
inline bool CLReadPointer<T>::operator!=(CLReadPointer & anotherOne)
{
	return !operator==(anotherOne);
}

template<typename T>
CLReadPointer<T>::~CLReadPointer()
{
}

template<typename T>
inline T * CLReadPointer<T>::operator->()
{
	if (CLTransactionAPIs::IsInReading())
	{
		return (T *)m_readOnlyPointer.operator ->();
	}
	else
	{
		return (T *)m_readWritePointer.operator ->();
	}
}

template<typename T>
inline T * CLReadPointer<T>::Get()
{
	if (CLTransactionAPIs::IsInReading())
	{
		return (T *)m_readOnlyPointer.Get();
	}
	else
	{
		return (T *)m_readWritePointer.Get();
	}
}

TRANSACTIONLIB_NS_END
#endif
