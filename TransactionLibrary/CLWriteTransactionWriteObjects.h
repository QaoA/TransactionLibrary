#ifndef __WRITE_TRANSACTION_WRITE_OBJECTS_H__
#define __WRITE_TRANSACTION_WRITE_OBJECTS_H__

#include "LSATimeStamp.h"
#include <list>
#include <cassert>

class CLWriteTransaction;
class CLTransactionalObject;
class CLLogItemsSet;

class CLWriteTransactionWriteObjects
{
public:
	CLWriteTransactionWriteObjects();
	~CLWriteTransactionWriteObjects();

public:
	inline void AddObject(CLTransactionalObject * pObject);
	void Commit(CLLogItemsSet & itemSet, LSATimeStamp commitTime);
	void CloseAll(CLWriteTransaction * pOwner);
	void Abort(CLWriteTransaction * pOwner);

private:
	std::list<CLTransactionalObject *> m_writeSet;
};

inline void CLWriteTransactionWriteObjects::AddObject(CLTransactionalObject * pObject)
{
	assert(pObject);
	m_writeSet.push_back(pObject);
}

#endif