#ifndef __GABAGE_COLLECTOR_H__
#define __GABAGE_COLLECTOR_H__

#include <pthread.h>
#include <map>
#include <list>
#include "CLMutex.h"
#include "CLDeletedObjectSet.h"
#include "TransactionLibraryNameSpace.h"
#include "CLQuiescentStateManager.h"

TRANSACTIONLIB_NS_BEGIN

class CLGarbageCollector
{
public:
	CLGarbageCollector();
	~CLGarbageCollector();

public:
	static CLGarbageCollector & GetInstance();

public:
	void NotifyTransactionBegin();
	void NotifyTransactionEnd();
	void CollectGarbage(void * gabage, DestructFunc releaseFunc);
	
private:
	CLMutex m_lock;
	std::list<CLDeletedObjectSet *> m_waitingForDeleteObjectSets;
	CLQuiescentStateManager m_quiescentStateManager;
};

TRANSACTIONLIB_NS_END
#endif