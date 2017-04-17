#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__

class CLTransactionAbort;

typedef void(*TransactionFunc)(void * arg);

class CLTransaction
{
public:
	CLTransaction();
	virtual ~CLTransaction();

public:
	virtual void Initialize() = 0;
	virtual void Uninitialize() = 0;
	virtual void OnCommit() = 0;
	virtual void OnAbort(CLTransactionAbort &) = 0;
	
public:
	void RunTransaction(TransactionFunc func, void * arg);
};

#endif