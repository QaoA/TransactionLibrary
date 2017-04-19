#ifndef __ATOMIC_OPERATION_H__
#define __ATOMIC_OPERATION_H__

#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

#ifdef _WIN64 //���õĶ��壬������VS�ϲ�ʹ�ô��벹ȫ����
#define BoolCompareAndSet(valuePtr,oldValue,newValue) false
#define IncreaseAndFetch(valuePtr) 0
#else
#define BoolCompareAndSet(valuePtr,oldValue,newValue) __sync_bool_compare_and_swap(valuePtr,oldValue,newValue)
#define IncreaseAndFetch(valuePtr) __sync_add_and_fetch(valuePtr,1)
#endif

TRANSACTIONLIB_NS_END
#endif