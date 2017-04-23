#include "CLLogItemsSet.h"
#include "CLLogItem.h"
#include "CLLogArea.h"
#include <cassert>

TRANSACTIONLIB_NS_BEGIN

using NVMMalloc::CLLogItem;

CLLogItemsSet::CLLogItemsSet()
{
}

CLLogItemsSet::~CLLogItemsSet()
{
}

void CLLogItemsSet::Reset()
{
	assert(m_itemsSet.empty());
}

void CLLogItemsSet::AddItem(CLLogItem * item)
{
	m_itemsSet.push_back(item);
}

void CLLogItemsSet::AddItem(void * pNVMAddress, unsigned long size, void * pValue)
{
	m_itemsSet.push_back(new CLLogItem(pNVMAddress, size, pValue));
}

void CLLogItemsSet::AddItem(void * pNVMAddress, unsigned long size, char * value)
{
	m_itemsSet.push_back(new CLLogItem(pNVMAddress, size, value));
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
		pItem->SetValues();
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

TRANSACTIONLIB_NS_END