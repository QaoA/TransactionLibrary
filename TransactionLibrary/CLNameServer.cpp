#include "CLNameServer.h"
#include "SLNVMNameAddressPair.h"
#include "CLCriticalSection.h"
#include "NVMMalloc.h"
#include <cstring>

TRANSACTIONLIB_NS_BEGIN

CLNameServer::CLNameServer(NVMMalloc::CLNameServerArea * pArea):
m_pArea(pArea)
{
	unsigned int maxIndex = pArea->GetAreaSize() / sizeof(SLNVMNameAddressPair);
	SLNVMNameAddressPair * objectsArray = (SLNVMNameAddressPair *)(pArea->GetAreaStartAddress());
	for (int i = 0; i < maxIndex; ++i)
	{
		if (objectsArray[i].m_strName[0] == '\0')
		{
			m_availableObjectsIndex.push_back(i);
		}
		else
		{
			m_objectsLookupList.push_back(i);
		}
	}
}

CLNameServer::~CLNameServer()
{
}

CLNameServer & CLNameServer::GetInstance()
{
	static CLNameServer instance(NVMMalloc::GetNameServer());
	return instance;
}

SLNVMNameAddressPair * CLNameServer::Get()
{
	CLCriticalSection cs(&m_lock);
	unsigned int index = m_availableObjectsIndex.front();
	m_availableObjectsIndex.pop_front();
	return (SLNVMNameAddressPair *)(m_pArea->GetAreaStartAddress()) + index;
}

void CLNameServer::Free(SLNVMNameAddressPair * pObject)
{
	CLCriticalSection cs(&m_lock);
	unsigned int index = pObject - (SLNVMNameAddressPair *)(m_pArea->GetAreaStartAddress());
	if (pObject->m_strName[0] == '\0')
	{
		m_availableObjectsIndex.push_back(index);
	}
	else
	{
		m_objectsLookupList.push_back(index);
	}
}

SLNVMNameAddressPair * CLNameServer::Find(char * strName)
{
	CLCriticalSection cs(&m_lock);
	SLNVMNameAddressPair * objectsArray = (SLNVMNameAddressPair *)(m_pArea->GetAreaStartAddress());
	for (unsigned int index : m_objectsLookupList)
	{
		if (strcmp(strName, objectsArray[index].m_strName) == 0)
		{
			return &objectsArray[index];
		}
	}
	return nullptr;
}

SLNVMNameAddressPair * CLNameServer::FindAndGet(char * strName)
{
	CLCriticalSection cs(&m_lock);
	SLNVMNameAddressPair * objectsArray = (SLNVMNameAddressPair *)(m_pArea->GetAreaStartAddress());
	for (std::list<unsigned int>::iterator it = m_objectsLookupList.begin(); it != m_objectsLookupList.end();++it)
	{
		if (strcmp(strName, objectsArray[*it].m_strName) == 0)
		{
			unsigned index = *it;
			m_objectsLookupList.erase(it);
			return &objectsArray[index];
		}
	}
	return nullptr;
}

TRANSACTIONLIB_NS_END