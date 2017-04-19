#ifndef __TRANSACTIONAL_OBJECT_LOOKUP_TABLE_H__
#define __TRANSACTIONAL_OBJECT_LOOKUP_TABLE_H__

#include "CLRadixTree.h"
#include "CLMutex.h"
#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

class CLTransactionalObject;

class CLTransactionalObjectLookupTable
{
private:
	struct SLTransactionalObjectWrapper
	{
		CLTransactionalObject * m_pObject;
		unsigned long m_referenceCount;
	};

private:
	CLTransactionalObjectLookupTable();
	CLTransactionalObjectLookupTable(const CLTransactionalObjectLookupTable &);
	CLTransactionalObjectLookupTable & operator=(const CLTransactionalObjectLookupTable &);

public:
	~CLTransactionalObjectLookupTable();

public:
	static CLTransactionalObjectLookupTable & GetInstance();

public:
	CLTransactionalObject * GetOrCreate(void * pNVMObject, void * transactionalObjectCreateArgs);
	void Put(void * pNVMObject);

private:
	CLRadixTree<SLTransactionalObjectWrapper> m_objectTree;
	CLMutex m_lock;
};

TRANSACTIONLIB_NS_END
#endif