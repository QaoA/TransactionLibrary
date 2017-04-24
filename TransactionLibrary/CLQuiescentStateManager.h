#ifndef __CLQUIESCENT_STATE_MANAGER_H__
#define __CLQUIESCENT_STATE_MANAGER_H__

#include <pthread.h>
#include <map>
#include <list>
#include <ctime>
#include <cassert>
#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

class CLQuiescentStateManager
{
private:
	typedef std::map<pthread_t, int> ThreadsVersionsSnapshot;

public:
	CLQuiescentStateManager();
	~CLQuiescentStateManager();

public:
	void InceaseCurrentThreadVersion();
	clock_t GetGracePeriodLowLimit();
	inline void PushCurrentThreadsVersionsSnapshot();

private:
	inline bool IsQuiescentStateAppeared(int firstVersion, int secondVersion);

private:
	std::map<pthread_t, int> m_curThreadsVersions;
	std::list<std::pair<clock_t, ThreadsVersionsSnapshot> > m_allThreadsVersionsSnapshots;

private:
	bool CompareTwoThreadsVersionSnapshot(ThreadsVersionsSnapshot snapshot);
};

void CLQuiescentStateManager::PushCurrentThreadsVersionsSnapshot()
{
	clock_t curTimeStamp = clock();
	m_allThreadsVersionsSnapshots.push_back(std::make_pair(curTimeStamp, m_curThreadsVersions));
}

bool CLQuiescentStateManager::IsQuiescentStateAppeared(int firstVersion, int secondVersion)
{
	assert(firstVersion >= secondVersion);
	assert(!(firstVersion == 0 && secondVersion == 0 && firstVersion == secondVersion));
	if ((firstVersion == secondVersion) && (firstVersion & 0x1))
	{
		return false;
	}
	if (secondVersion == 0 && firstVersion == 1)
	{
		return false;
	}
	return true;
}

TRANSACTIONLIB_NS_END

#endif

