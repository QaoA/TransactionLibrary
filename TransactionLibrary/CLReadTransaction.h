#ifndef __READ_TRANSACTION_H__
#define __READ_TRANSACTION_H__

#include "CLTransaction.h"
#include "CLSnapShot.h"
#include "CLReadTransactionReadedObjects.h"

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
	CLReadedObject * OpenObject(void * pUserObject, SLUserObjectInfo * pUserObjectInfo);

private:
	CLSnapShot m_snapShot;
	CLReadTransactionReadedObjects m_readSet;
};

#endif