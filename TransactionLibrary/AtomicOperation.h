#ifndef __ATOMIC_OPERATION_H__
#define __ATOMIC_OPERATION_H__

#ifdef _WIN64 //无用的定义，用于在VS上不使得代码补全报错
#define BoolCompareAndSet(valuePtr,oldValue,newValue) false
#define IncreaseAndFetch(valuePtr) 0
#else
#define BoolCompareAndSet(valuePtr,oldValue,newValue) __sync_bool_compare_and_swap(valuePtr,oldValue,newValue)
#define IncreaseAndFetch(valuePtr) __sync_add_and_fetch(valuePtr,1)
#endif

#endif