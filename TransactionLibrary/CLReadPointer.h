#ifndef __READ_POINTER_H__
#define __READ_POINTER_H__

#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

class CLTransactionalObject;

template<typename T>
class CLReadPointer
{
public:
	CLReadPointer(void * pNVMUserObject);
	~CLReadPointer();

public:
	const T * operator->() const;

private:
};

template<typename T>
CLReadPointer<T>::CLReadPointer(void * pNVMUserObject)
{
}

template<typename T>
CLReadPointer<T>::~CLReadPointer()
{
}

template<typename T>
const T * CLReadPointer<T>::operator->() const
{

}

TRANSACTIONLIB_NS_END
#endif