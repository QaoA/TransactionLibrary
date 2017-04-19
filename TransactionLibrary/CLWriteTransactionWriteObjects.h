#ifndef __WRITE_TRANSACTION_WRITE_OBJECTS_H__
#define __WRITE_TRANSACTION_WRITE_OBJECTS_H__

#include "TransactionLibraryNameSpace.h"
#include "LSATimeStamp.h"
#include <set>
#include <cassert>

TRANSACTIONLIB_NS_BEGIN

class CLWriteTransaction;
class CLTransactionalObject;
class CLLogItemsSet;

class CLWriteTransactionWriteObjects
{
public:
	CLWriteTransactionWriteObjects();
	~CLWriteTransactionWriteObjects();

public:
	void Reset();
	void AddObject(CLTransactionalObject * pObject);
	void Commit(CLLogItemsSet & itemSet, LSATimeStamp commitTime);
	void Abort(CLWriteTransaction * pOwner);
	void CloseAll(CLWriteTransaction * pOwner);

private:
	std::set<CLTransactionalObject *> m_writeSet;
};

TRANSACTIONLIB_NS_END
#endif