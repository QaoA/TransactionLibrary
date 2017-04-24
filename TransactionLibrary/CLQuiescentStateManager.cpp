#include "CLQuiescentStateManager.h"

using namespace std;

TRANSACTIONLIB_NS_BEGIN

CLQuiescentStateManager::CLQuiescentStateManager()
{
}

CLQuiescentStateManager::~CLQuiescentStateManager()
{
}

void CLQuiescentStateManager::InceaseCurrentThreadVersion()
{
	pthread_t hThread;
	hThread = pthread_self();
	ThreadsVersionsSnapshot::iterator it = m_curThreadsVersions.find(hThread);
	if (it == m_curThreadsVersions.end())
	{
		m_curThreadsVersions.insert(std::make_pair(hThread, 1));
		return;
	}
	(it->second)++;
}

clock_t CLQuiescentStateManager::GetGracePeriodLowLimit()
{
	clock_t lowlimt = 0;
	for (list<std::pair<clock_t, ThreadsVersionsSnapshot> >::iterator it = m_allThreadsVersionsSnapshots.begin(); it != m_allThreadsVersionsSnapshots.end();)
	{
		if (CompareTwoThreadsVersionSnapshot(it->second))
		{
			lowlimt = it->first;
			++it;
			m_allThreadsVersionsSnapshots.pop_front();
		}
		else
		{
			break;
		}
	}
	return lowlimt;
}

bool CLQuiescentStateManager::CompareTwoThreadsVersionSnapshot(ThreadsVersionsSnapshot snapshot)
{
	ThreadsVersionsSnapshot::iterator it_2;
	for (ThreadsVersionsSnapshot::iterator it_1 = m_curThreadsVersions.begin(); it_1 != m_curThreadsVersions.end(); ++it_1)
	{
		if ((it_2 = snapshot.find(it_1->first)) != snapshot.end())
		{
			if (!IsQuiescentStateAppeared(it_1->second, it_2->second))
			{
				return false;
			}
		}
		else
		{
			if (!IsQuiescentStateAppeared(it_1->second, 0))
			{
				return false;
			}
		}
	}
	return true;
}

TRANSACTIONLIB_NS_END