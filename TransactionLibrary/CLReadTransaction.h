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
	CLReadedObject * OpenObject(void * pUserObject, SLUserObjectInfo * pUserObjectInfo);

private:
	CLSnapShot m_snapShot;
	CLReadTransactionReadedObjects m_readSet;
};

#endif