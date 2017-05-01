#ifndef __TRANSACTION_APIS_H__
#define __TRANSACTION_APIS_H__

#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

typedef void(*TransactionFunc)(void * arg);

class CLTransactionAPIs
{
public:
	static bool IsInReading();
	static bool IsInWriting();
	static bool RunReadTransaction(TransactionFunc func, void * arg);
	static bool RunWriteTransaction(TransactionFunc func, void * arg);
	static bool SetAddressByName(char * strName, void * address);
	static void * GetAddressByName(char * strName);
};

TRANSACTIONLIB_NS_END
#endif