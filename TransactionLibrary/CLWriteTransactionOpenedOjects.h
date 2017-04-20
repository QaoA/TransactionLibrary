#ifndef __WRITE_TRANSACTION_OPENED_OBJECTS_H__
#define __WRITE_TRANSACTION_OPENED_OBJECTS_H__

#include "LSATimeStamp.h"
#include "TransactionLibraryNameSpace.h"
#include "EMObjectOpenMode.h"
#include <map>

TRANSACTIONLIB_NS_BEGIN

class CLTransactionalObject;
class CLWriteTransaction;
class CLLogItemsSet;

class CLWriteTransactionOpenedOjects
{
public:
	CLWriteTransactionOpenedOjects();
	~CLWriteTransactionOpenedOjects();

public:
	CLTransactionalObject * Find(void * pUserObjectNVMAddress);
	void AddObject(CLTransactionalObject * pObject);
	void Commit(CLWriteTransaction * pOwner, CLLogItemsSet & itemSet, LSATimeStamp commitTime);
	void Abort(CLWriteTransaction * pOwner);
	void Close(CLWriteTransaction * pOwner);
	void Reset();

private:
	std::map<void *, CLTransactionalObject *> m_objectSet;
};

TRANSACTIONLIB_NS_END
#endif