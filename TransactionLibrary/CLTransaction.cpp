#include "CLTransaction.h"
#include "CLTransactionAbort.h"

TRANSACTIONLIB_NS_BEGIN

CLTransaction::CLTransaction()
{
}

CLTransaction::~CLTransaction()
{
}

bool CLTransaction::RunTransaction(TransactionFunc func, void * arg)
{
	bool ret = false;
	Initialize();
	try
	{
		func(arg);
		OnCommit();
		ret = true;
	}
	catch (CLTransactionAbort & transactionAbort)
	{
		OnAbort(transactionAbort);
	}
	Uninitialize();
	return ret;
}

TRANSACTIONLIB_NS_END