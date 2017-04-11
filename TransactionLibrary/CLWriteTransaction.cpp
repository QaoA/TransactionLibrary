#include "CLWriteTransaction.h"
#include "CLTransactionalObject.h"
#include <cassert>

CLWriteTransaction::CLWriteTransaction()
{
}

CLWriteTransaction::~CLWriteTransaction()
{
}

CLTransactionalObject * CLWriteTransaction::OpenObjectRead(void * pUserObject)
{
	assert(pUserObject);
	CLTransactionalObject::
}