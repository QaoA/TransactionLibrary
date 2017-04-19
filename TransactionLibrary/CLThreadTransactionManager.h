#ifndef __THREAD_TRANSACTION_MANAGER_H__
#define __THREAD_TRANSACTION_MANAGER_H__

#include "CLReadTransaction.h"
#include "CLWriteTransaction.h"
#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

class CLThreadTransactionManager
{
private:
	CLThreadTransactionManager();
	CLThreadTransactionManager(const CLThreadTransactionManager &);
	CLThreadTransactionManager & operator=(const CLThreadTransactionManager &);

public:
	~CLThreadTransactionManager();

public:
	static CLThreadTransactionManager & GetInstance();

public:
	inline CLReadTransaction & GetReadTransaction();
	inline CLWriteTransaction & GetWriteTransaction();

private:
	CLReadTransaction m_readTransaction;
	CLWriteTransaction m_writeTransaction;
};

inline CLReadTransaction & CLThreadTransactionManager::GetReadTransaction()
{
	return m_readTransaction;
}

inline CLWriteTransaction & CLThreadTransactionManager::GetWriteTransaction()
{
	return m_writeTransaction;
}

TRANSACTIONLIB_NS_END

#endif