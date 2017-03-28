#include "CLThreadTransactionManager.h"

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
