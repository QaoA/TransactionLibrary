#ifndef __WRITE_TRANSACTION_READ_OBJECTS_H__
#define __WRITE_TRANSACTION_READ_OBJECTS_H__

#include "TransactionLibraryNameSpace.h"
#include <set>
#include <cassert>

TRANSACTIONLIB_NS_BEGIN

class CLTransactionalObject;
class CLWriteTransaction;

class CLWriteTransactionReadObjects
{
public:
	CLWriteTransactionReadObjects();
	~CLWriteTransactionReadObjects();

public:
	void Reset();
	void AddObject(CLTransactionalObject * pObject);
	bool RemoveObject(CLTransactionalObject * pObject);
	void Commit(CLWriteTransaction * pOwner);
	void Abort(CLWriteTransaction * pOwner);

private:
	std::set<CLTransactionalObject *> m_readSet;
};

TRANSACTIONLIB_NS_END
#endif