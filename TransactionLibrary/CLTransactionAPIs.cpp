#include "CLTransactionAPIs.h"
#include "CLThreadTransactionManager.h"

TRANSACTIONLIB_NS_BEGIN

bool CLTransactionAPIs::IsInReading()
{
	return CLThreadTransactionManager::IsInRead();
}

bool CLTransactionAPIs::IsInWriting()
{
	return CLThreadTransactionManager::IsInWrite();
}

bool CLTransactionAPIs::RunReadTransaction(TransactionFunc func, void * arg)
{
	return CLThreadTransactionManager::GetInstance().RunReadTransaction(func, arg);
}

bool CLTransactionAPIs::RunWriteTransaction(TransactionFunc func, void * arg)
{
	return CLThreadTransactionManager::GetInstance().RunWriteTransaction(func, arg);
}

bool CLTransactionAPIs::SetAddressByName(char * strName, void * address)
{
	return CLThreadTransactionManager::GetWriteTransaction()->SetAddressByName(strName, address);
}

void * CLTransactionAPIs::GetAddressByName(char * strName)
{
	if (IsInReading())
	{
		return CLThreadTransactionManager::GetReadTransaction()->GetAddressByName(strName);
	}
	else
	{
		return CLThreadTransactionManager::GetWriteTransaction()->GetAddressByName(strName);
	}
}

TRANSACTIONLIB_NS_END