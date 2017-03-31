#ifndef __READ_TRANSACTION_H__
#define __READ_TRANSACTION_H__

#include "CLTransaction.h"
#include "CLSnapShot.h"
#include "CLReadTransactionReadedObjects.h"

class CLReadTransaction : CLTransaction
{
public:
	CLReadTransaction();
	virtual ~CLReadTransaction();

public:
	inline CLSnapShot & GetSnapShot();
	inline CLReadTransactionReadedObjects & GetReadSet();

private:
	CLSnapShot m_snapShot;
	CLReadTransactionReadedObjects m_readSet;
};

inline CLSnapShot & CLReadTransaction::GetSnapShot()
{
	return m_snapShot;
}

inline CLReadTransactionReadedObjects & CLReadTransaction::GetReadSet()
{
	return m_readSet;
}

#endif