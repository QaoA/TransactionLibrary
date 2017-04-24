#include "CLGarbageCollector.h"
#include "CLCriticalSection.h"

TRANSACTIONLIB_NS_BEGIN
	
static __thread CLDeletedObjectSet * m_currrentThreadDeletedObjectSet = nullptr;

CLGarbageCollector::CLGarbageCollector()
{
}

CLGarbageCollector::~CLGarbageCollector()
{
}

CLGarbageCollector & CLGarbageCollector::GetInstance()
{
	static CLGarbageCollector instance;
	return instance;
}

void CLGarbageCollector::NotifyTransactionBegin()
{
	CLCriticalSection cs(&m_lock);
	m_quiescentStateManager.InceaseCurrentThreadVersion();
}

void CLGarbageCollector::NotifyTransactionEnd()
{
	CLDeletedObjectSet * currentObjectSet = m_currrentThreadDeletedObjectSet;
	if (currentObjectSet != nullptr)
	{
		currentObjectSet->SetTimeStamp();
	}
	m_currrentThreadDeletedObjectSet = nullptr;

	CLCriticalSection cs(&m_lock);
	m_quiescentStateManager.InceaseCurrentThreadVersion();
	m_quiescentStateManager.PushCurrentThreadsVersionsSnapshot();
	if (currentObjectSet)
	{
		m_waitingForDeleteObjectSets.push_back(currentObjectSet);
	}
	clock_t lowLimit = m_quiescentStateManager.GetGracePeriodLowLimit();
	for (std::list<CLDeletedObjectSet *>::iterator it = m_waitingForDeleteObjectSets.begin(); it != m_waitingForDeleteObjectSets.end();)
	{
		if ((*it)->GetTimeStamp() <= lowLimit)
		{
			(*it)->ReleaseAll();
			delete(*it);
			++it;
			m_waitingForDeleteObjectSets.pop_front();
		}
		else
		{
			break;
		}
	}
}

void CLGarbageCollector::CollectGarbage(void * gabage, DestructFunc releaseFunc)
{
	CLDeletedObjectSet * currentObjectSet = m_currrentThreadDeletedObjectSet;
	if (currentObjectSet == nullptr)
	{
		currentObjectSet = new CLDeletedObjectSet();
		m_currrentThreadDeletedObjectSet = currentObjectSet;
	}
	currentObjectSet->AddObject(gabage, releaseFunc);
}

TRANSACTIONLIB_NS_END