#include "CLTransaction.h"
#include "CLTransactionAbort.h"

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