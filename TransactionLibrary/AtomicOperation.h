#ifndef __ATOMIC_OPERATION_H__
#define __ATOMIC_OPERATION_H__

#define BoolCompareAndSet(valuePtr,oldValue,newValue) __sync_bool_compare_and_swap(valuePtr,oldValue,newValue)
#define IncreaseAndFetch(valuePtr) __sync_add_and_fetch(valuePtr,1)

#endif