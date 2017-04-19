#ifndef __GABAGE_COLLECTOR_H__
#define __GABAGE_COLLECTOR_H__

#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

typedef void (*DestructFunc)(void *);

class CLGarbageCollector
{
public:
	CLGarbageCollector();
	~CLGarbageCollector();

public:
	static CLGarbageCollector & GetInstance();

public:
	void CollectGarbage(void * gabage, DestructFunc releaseFunc);
};

TRANSACTIONLIB_NS_END
#endif