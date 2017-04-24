#include "CLThreadTransactionManager.h"
#include <cassert>

TRANSACTIONLIB_NS_BEGIN

static __thread CLReadTransaction * m_pReadTransaction = nullptr;
static __thread CLWriteTransaction * m_pWriteTransaction = nullptr;

CLThreadTransactionManager::CLThreadTransactionManager():
m_threadTransactions(ReleaseThreadTransactions)
{
}

CLThreadTransactionManager::~CLThreadTransactionManager()
{
}

CLThreadTransactionManager & CLThreadTransactionManager::GetInstance()
{
	static CLThreadTransactionManager instance;
	return instance;
}

void CLThreadTransactionManager::ReleaseThreadTransactions(void * pThreadTransactions)
{
	delete(SLThreadTransactions *)(pThreadTransactions);
}

CLReadTransaction * CLThreadTransactionManager::GetReadTransaction()
{
	assert(m_pReadTransaction);
	return m_pReadTransaction;
}

CLWriteTransaction * CLThreadTransactionManager::GetWriteTransaction()
{
	assert(m_pWriteTransaction);
	return m_pWriteTransaction;
}

void CLThreadTransactionManager::RunReadTransaction(TransactionFunc func, void * arg)
{
	m_pReadTransaction = &GetThreadTransactions()->m_readTransaction;
	m_pReadTransaction->RunTransaction(func, arg);
	m_pReadTransaction = nullptr;
}

void CLThreadTransactionManager::RunWriteTransaction(TransactionFunc func, void * arg)
{
	m_pWriteTransaction = &GetThreadTransactions()->m_writeTransaction;
	m_pWriteTransaction->RunTransaction(func, arg);
	m_pWriteTransaction = nullptr;
}

CLThreadTransactionManager::SLThreadTransactions * CLThreadTransactionManager::GetThreadTransactions()
{
	SLThreadTransactions * pTransactions = m_threadTransactions.Get();
	if (pTransactions == nullptr)
	{
		pTransactions = new SLThreadTransactions();
		m_threadTransactions.Set(pTransactions);
	}
	return pTransactions;
}

TRANSACTIONLIB_NS_END