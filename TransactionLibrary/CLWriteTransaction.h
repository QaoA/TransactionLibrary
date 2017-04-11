#ifndef __WRITE_TRANSACTION_H__
#define __WRITE_TRANSACTION_H__

#include "CLTransaction.h"
#include "CLWriteTransactionReadObjects.h"
#include "CLWriteTransactionWriteObjects.h"

class CLWriteTransaction : public CLTransaction
{
public:
	CLWriteTransaction();
	virtual ~CLWriteTransaction();

public:
	CLTransactionalObject * OpenObjectRead(void * pUserObject);

public:
	CLWriteTransactionReadObjects m_readSet;
	CLWriteTransactionWriteObjects m_writeSet;
};

#endif