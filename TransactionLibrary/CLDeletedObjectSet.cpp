#include "CLDeletedObjectSet.h"

TRANSACTIONLIB_NS_BEGIN

CLDeletedObjectSet::CLDeletedObjectSet()
{
}

CLDeletedObjectSet::~CLDeletedObjectSet()
{
}

void CLDeletedObjectSet::AddObject(void * pObject, DestructFunc releaseFunc)
{
	m_objectSet.push_back(SLDeletedObject{ pObject,releaseFunc });
}

void CLDeletedObjectSet::ReleaseAll()
{
	for (std::list<SLDeletedObject>::iterator it = m_objectSet.begin(); it != m_objectSet.end(); ++it)
	{
		it->m_releaseFunc(it->m_pObject);
		m_objectSet.erase(it++);
	}
}

void CLDeletedObjectSet::SetTimeStamp()
{
	m_timeStamp = clock();
}

clock_t CLDeletedObjectSet::GetTimeStamp()
{
	return m_timeStamp;
}

TRANSACTIONLIB_NS_END