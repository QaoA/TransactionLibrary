#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__

#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

class CLTransactionAbort;

typedef void(*TransactionFunc)(void * arg);

class CLTransaction
{
public:
	CLTransaction();
	virtual ~CLTransaction();

protected:
	virtual void Initialize() = 0;
	virtual void Uninitialize() = 0;
	virtual void OnCommit() = 0;
	virtual void OnAbort(CLTransactionAbort &) = 0;
	
public:
	bool RunTransaction(TransactionFunc func, void * arg);
};

TRANSACTIONLIB_NS_END
#endif