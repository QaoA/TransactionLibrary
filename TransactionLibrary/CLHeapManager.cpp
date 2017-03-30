#include "CLHeapManager.h"
#include "CLCriticalSection.h"
#include <cassert>

using namespace std;

CLHeapManager::CLHeapManager()
{
}

CLHeapManager::~CLHeapManager()
{
}

CLHeapManager * CLHeapManager::GetInstance()
{
	static CLHeapManager instance;
	return &instance;
}

void CLHeapManager::NotifyTransactionBegin()
{
	CLCriticalSection cs(&m_mutex);
	m_manager.InceaseCurrentThreadVersion();
}

void CLHeapManager::NotifyTransactionEnd(CLDeleteSet ** deleteSet)
{
	if (deleteSet != nullptr)
	{
		assert(*deleteSet);
		(*deleteSet)->SetTimeStamp();
	}
	{
		CLCriticalSection cs(&m_mutex);
		m_manager.InceaseCurrentThreadVersion();
		m_manager.PushCurrentThreadsVersionsSnapshot();
		if (deleteSet != nullptr)
		{
			m_waitingForDeleteObject.push_back(*deleteSet);
			*deleteSet = nullptr;
		}
		clock_t lowLimit = m_manager.GetGracePeriodLowLimit();
		for (list<CLDeleteSet *>::iterator it = m_waitingForDeleteObject.begin(); it != m_waitingForDeleteObject.end();)
		{
			if ((*it)->GetTimeStamp() <= lowLimit)
			{
				(*it)->ReleaseAll();
				++it;
				m_waitingForDeleteObject.pop_front();
			}
			else
			{
				break;
			}
		}
	}

}

void CLHeapManager::NotifyTransactionEnd()
{
	NotifyTransactionEnd(nullptr);
}