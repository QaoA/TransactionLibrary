#ifndef __THREAD_TRANSACTION_MANAGER_H__
#define __THREAD_TRANSACTION_MANAGER_H__

#include "CLTLSVariable.h"
#include "CLReadTransaction.h"
#include "CLWriteTransaction.h"
#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

class CLThreadTransactionManager
{
private:
	struct SLThreadTransactions
	{
		CLReadTransaction m_readTransaction;
		CLWriteTransaction m_writeTransaction;
	};
	
private:
	CLThreadTransactionManager();
	CLThreadTransactionManager(const CLThreadTransactionManager &);
	CLThreadTransactionManager & operator=(const CLThreadTransactionManager &);

public:
	~CLThreadTransactionManager();
	
public:
	static CLReadTransaction * GetReadTransaction();
	static CLWriteTransaction * GetWriteTransaction();
	static CLThreadTransactionManager & GetInstance();
	
private:
	static void ReleaseThreadTransactions(void * pThreadTransactions);
	
public:
	void RunReadTransaction(TransactionFunc func, void * arg);
	void RunWriteTransaction(TransactionFunc func, void * arg);
	
private:
	SLThreadTransactions * GetThreadTransactions();

private:
	CLTLSVariable<SLThreadTransactions> m_threadTransactions;
};

TRANSACTIONLIB_NS_END

#endif