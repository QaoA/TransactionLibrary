#ifndef __READ_ONLY_POINTER_IMPLEMENTATOR_H__
#define __READ_ONLY_POINTER_IMPLEMENTATOR_H__

#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

struct SLUserObjectInfo;
struct SLObjectVersion;

class CLReadOnlyPointerImplementator
{
public:
	CLReadOnlyPointerImplementator();
	~CLReadOnlyPointerImplementator();

public:
	void Set(void * pNVMUserObject, SLUserObjectInfo * pUserObjectInfo);
	void Set(CLReadOnlyPointerImplementator & anotherOne);
	void * operator->();
	void * Get();

private:
	SLObjectVersion * m_readedVersion;
};

TRANSACTIONLIB_NS_END
#endif