#include "CLTransactionalObjectLookupTable.h"
#include "CLTransactionalObject.h"
#include <cassert>

CLTransactionalObjectLookupTable::CLTransactionalObjectLookupTable()
{
}

CLTransactionalObjectLookupTable::~CLTransactionalObjectLookupTable()
{
}

CLTransactionalObjectLookupTable & CLTransactionalObjectLookupTable::GetInstance()
{
	static CLTransactionalObjectLookupTable instance;
	return instance;
}
