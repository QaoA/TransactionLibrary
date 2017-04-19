#include "CLThreadTransactionManager.h"

TRANSACTIONLIB_NS_BEGIN

CLThreadTransactionManager::CLThreadTransactionManager()
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

TRANSACTIONLIB_NS_END