#include "CLTransaction.h"
#include "CLTransactionAbort.h"

TRANSACTIONLIB_NS_BEGIN

CLTransaction::CLTransaction()
{
}

CLTransaction::~CLTransaction()
{
}

void CLTransaction::RunTransaction(TransactionFunc func, void * arg)
{
	Initialize();
	try
	{
		func(arg);
		OnCommit();
	}
	catch (CLTransactionAbort & transactionAbort)
	{
		OnAbort(transactionAbort);
	}
	Uninitialize();
}

TRANSACTIONLIB_NS_END