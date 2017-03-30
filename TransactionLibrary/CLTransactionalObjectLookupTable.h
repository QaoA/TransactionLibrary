#ifndef __TRANSACTIONAL_OBJECT_LOOKUP_TABLE_H__
#define __TRANSACTIONAL_OBJECT_LOOKUP_TABLE_H__

#include "CLRadixTree.h"

class CLTransactionalObject;

class CLTransactionalObjectLookupTable
{
public:
	CLTransactionalObjectLookupTable();
	~CLTransactionalObjectLookupTable();

public:
	inline CLTransactionalObject * FindOrCreate(void * pNVMObject, void * transactionalObjectCreateArgs);
	inline CLTransactionalObject * Remove(void * pNVMObject);

private:
	CLRadixTree<CLTransactionalObject> m_objectTree;
};

inline CLTransactionalObject * CLTransactionalObjectLookupTable::FindOrCreate(void * pNVMObject, void * transactionalObjectCreateArgs)
{
	assert(pNVMObject);
	return m_objectTree.GetOrCreate(pNVMObject, CLTransactionalObject::MakeATransactionalObject, transactionalObjectCreateArgs);
}

inline CLTransactionalObject * CLTransactionalObjectLookupTable::Remove(void * pNVMObject)
{
	assert(pNVMObject);
	return m_objectTree.GetAndRemove(pNVMObject);
}


#endif