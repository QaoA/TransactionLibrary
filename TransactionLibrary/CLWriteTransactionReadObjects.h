#ifndef __WRITE_TRANSACTION_READ_OBJECTS_H__
#define __WRITE_TRANSACTION_READ_OBJECTS_H__

#include <set>
#include <cassert>

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

#endif