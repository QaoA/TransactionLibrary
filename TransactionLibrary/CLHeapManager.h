#ifndef __CLHEAP_MANAGER_H__
#define __CLHEAP_MANAGER_H__

#include <list>
#include "CLMutex.h"
#include "CLQuiescentStateManager.h"

class CLDeleteSet;

class CLHeapManager
{
public:
	~CLHeapManager();

private:
	CLHeapManager();
	CLHeapManager(const CLHeapManager &);
	CLHeapManager operator=(const CLHeapManager &);

public:
	static CLHeapManager * GetInstance();

public:
	void NotifyTransactionBegin();
	void NotifyTransactionEnd();
	void NotifyTransactionEnd(CLDeleteSet ** deleteSet);

private:
	CLMutex m_mutex;
	CLQuiescentStateManager m_manager;
	std::list<CLDeleteSet *> m_waitingForDeleteObject;
};

#endif