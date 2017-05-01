#ifndef __WRITE_POINTER_IMLEMENTATOR_H__
#define __WRITE_POINTER_IMLEMENTATOR_H__

#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

class CLTransactionalObject;
struct SLUserObjectInfo;
class CLReadWritePointerImplementator;

class CLWritePointerImplementator
{
public:
	CLWritePointerImplementator();
	~CLWritePointerImplementator();
	
public:
	void Set(void * pNVMUserObject, SLUserObjectInfo * pUserObjectInfo, bool IsNew = false);
	void Set(CLReadWritePointerImplementator & readWritePointer);
	void SetRelease();
	void * Get();
	void * operator->();
	
private:
	CLTransactionalObject * m_pObject;
};

TRANSACTIONLIB_NS_END
#endif