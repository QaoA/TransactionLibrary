#ifndef __TRANSACTIONAL_OBJECT_CREATE_ARGS_H__
#define __TRANSACTIONAL_OBJECT_CREATE_ARGS_H__

#include "TransactionLibraryNameSpace.h"
#include "EMObjectOpenMode.h"

TRANSACTIONLIB_NS_BEGIN

class CLWriteTransaction;
struct SLUserObjectInfo;

struct SLTransactionalObjectCreatArgs
{
	CLWriteTransaction * m_owner;
	void * m_pNVMUserObject;
	SLUserObjectInfo * m_pUserObjectInfo;
	EMObjectOpenMode m_openMode;
};

TRANSACTIONLIB_NS_END
#endif