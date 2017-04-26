#ifndef __TRANSACTIONAL_OBJECT_LOOKUP_TABLE_H__
#define __TRANSACTIONAL_OBJECT_LOOKUP_TABLE_H__

#include "CLMutex.h"
#include "CLTransactionalObjectRadixTree.h"
#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

class CLTransactionalObject;

class CLTransactionalObjectLookupTable
{
private:
	CLTransactionalObjectLookupTable();
	CLTransactionalObjectLookupTable(const CLTransactionalObjectLookupTable &);
	CLTransactionalObjectLookupTable & operator=(const CLTransactionalObjectLookupTable &);

public:
	~CLTransactionalObjectLookupTable();

public:
	static CLTransactionalObjectLookupTable & GetInstance();

public:
	CLTransactionalObject * GetOrCreate(void * pNVMObject, SLTransactionalObjectCreatArgs * pArgs);
	void Put(CLTransactionalObject * pObject, bool CanDelete);

private:
	CLTransactionalObjectRadixTree m_objectTree;
	CLMutex m_lock;
};

TRANSACTIONLIB_NS_END
#endif