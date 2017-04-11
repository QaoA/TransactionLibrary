#include "CLLogItemsSet.h"
#include "CLLogItem.h"
#include "CLLogArea.h"

CLLogItemsSet::CLLogItemsSet()
{
}

CLLogItemsSet::~CLLogItemsSet()
{
}

void CLLogItemsSet::AddItem(CLLogItem * item)
{
	m_itemsSet.push_back(item);
}

void CLLogItemsSet::AddItem(void * pNVMAddress, unsigned long size, void * pValue)
{
	m_itemsSet.push_back(new CLLogItem(pNVMAddress, size, pValue));
}

void CLLogItemsSet::WriteLogs(CLLogArea & logArea)
{
	for (CLLogItem * pItem : m_itemsSet)
	{
		logArea.WriteLog(*pItem);
	}
}

void CLLogItemsSet::SetValues()
{
	for (CLLogItem * pItem : m_itemsSet)
	{
		memcpy(pItem->GetAddress(), pItem->GetValueAddress(), pItem->GetSize());
	}
}

void CLLogItemsSet::ReleaseAllItems()
{
	for (CLLogItem * pItem : m_itemsSet)
	{
		delete pItem;
	}
	m_itemsSet.erase(m_itemsSet.begin(), m_itemsSet.end());
}
