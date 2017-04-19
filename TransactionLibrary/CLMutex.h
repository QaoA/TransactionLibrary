#ifndef __MUTEX_H__
#define __MUTEX_H__

#include "ILLock.h"
#include "TransactionLibraryNameSpace.h"
#include <pthread.h>

TRANSACTIONLIB_NS_BEGIN

class CLMutex:public ILLock
{
public:
	CLMutex();
	~CLMutex();

public:
	virtual void Lock();
	virtual void Unlock();
	virtual bool TryLock();

private:
	pthread_mutex_t m_lock;
};

TRANSACTIONLIB_NS_END
#endif