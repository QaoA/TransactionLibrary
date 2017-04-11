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
	inline void AddObject(CLTransactionalObject * pObject);
	inline void RemoveObject(CLTransactionalObject * pObject);

public:
	void Commit(CLWriteTransaction * pOwner);
	void Abort(CLWriteTransaction * pOwner);

private:
	std::set<CLTransactionalObject *> m_readSet;
};

void CLWriteTransactionReadObjects::AddObject(CLTransactionalObject * pObject)
{
	assert(pObject);
	m_readSet.insert(pObject);
}

void CLWriteTransactionReadObjects::RemoveObject(CLTransactionalObject * pObject)
{
	m_readSet.erase(pObject);
}

#endif