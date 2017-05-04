#ifndef __TEST_FUNCTIONS_H__
#define __TEST_FUNCTIONS_H__

#include "CLTransactionAPIs.h"

using namespace NVMTransaction;

inline bool RunReadTransaction(TransactionFunc func, void * arg)
{
	return CLTransactionAPIs::RunReadTransaction(func, arg);
}

inline bool RunWriteTransaction(TransactionFunc func, void * arg)
{
	return CLTransactionAPIs::RunWriteTransaction(func, arg);
}

#endif
