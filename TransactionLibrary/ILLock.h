#ifndef __LOCK_H__
#define __LOCK_H__

#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

class ILLock
{
public:
	virtual void Lock() = 0;
	virtual void Unlock() = 0;
	virtual bool TryLock() = 0;
};

TRANSACTIONLIB_NS_END

#endif