#ifndef __WRITE_TRANSACTION_H__
#define __WRITE_TRANSACTION_H__

#include "CLTransaction.h"
#include "CLWriteTransactionReadObjects.h"
#include "CLWriteTransactionWriteObjects.h"
#include "CLLogItemsSet.h"
#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

struct SLUserObjectInfo;

class CLWriteTransaction : public CLTransaction
{
public:
	CLWriteTransaction();
	virtual ~CLWriteTransaction();

public:
	virtual void Initialize() override;
	virtual void Uninitialize() override;
	virtual void OnCommit() override;
	virtual void OnAbort(CLTransactionAbort &) override;

public:
	CLTransactionalObject * OpenObjectRead(void * pUserObject, SLUserObjectInfo * pUserObjectInfo);
	CLTransactionalObject * OpenObjectWrite(void * pUserObject, SLUserObjectInfo * pUserObjectInfo);
	void ConvertOpenModeReadToWrite(CLTransactionalObject * pObject);
	
public:
	CLWriteTransactionReadObjects m_readSet;
	CLWriteTransactionWriteObjects m_writeSet;
	CLLogItemsSet m_itemSet;
};

TRANSACTIONLIB_NS_END
#endif