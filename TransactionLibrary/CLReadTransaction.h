#ifndef __READ_TRANSACTION_H__
#define __READ_TRANSACTION_H__

#include "CLTransaction.h"
#include "CLSnapShot.h"
#include "CLReadTransactionReadedObjects.h"
#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

struct SLUserObjectInfo;

class CLReadTransaction : public CLTransaction
{
public:
	CLReadTransaction();
	virtual ~CLReadTransaction();

public:
	virtual void Initialize() override;
	virtual void Uninitialize() override;
	virtual void OnCommit() override;
	virtual void OnAbort(CLTransactionAbort &) override;

public:
	SLObjectVersion * OpenObject(void * pUserObject, SLUserObjectInfo * pUserObjectInfo);

private:
	CLSnapShot m_snapShot;
	CLReadTransactionReadedObjects m_readSet;
};

TRANSACTIONLIB_NS_END
#endif