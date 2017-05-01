#ifndef __READ_WRITE_POINTER_IMPLEMENTATOR_H__
#define __READ_WRITE_POINTER_IMPLEMENTATOR_H__

#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN
	
class CLTransactionalObject;
struct SLUserObjectInfo;
class CLWritePointerImplementator;

class CLReadWritePointerImplementator
{
	friend class CLWritePointerImplementator;
public:
	CLReadWritePointerImplementator();
	~CLReadWritePointerImplementator();

public:
	void Set(void * pNVMUserObject, SLUserObjectInfo * pUserObjectInfo);
	void Set(CLReadWritePointerImplementator & anotherOne);
	void * operator->();
	void * Get();

private:
	CLTransactionalObject * m_pObject;
};

TRANSACTIONLIB_NS_END
#endif