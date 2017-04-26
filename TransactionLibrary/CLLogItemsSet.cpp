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

void CLLogItemsSet::AddItem(void * pNVMAddress, unsigned long size, void * pValue)
{
	m_itemsSet.push_back(CLLogItem(pNVMAddress, size, pValue));
}

void CLLogItemsSet::AddItem(void * pNVMAddress, uint64_t value)
{
	m_itemsSet.push_back(CLLogItem(pNVMAddress, value));
}

void CLLogItemsSet::AddItem(void * pNVMAddress, uint32_t value)
{
	m_itemsSet.push_back(CLLogItem(pNVMAddress, value));
}

void CLLogItemsSet::AddItem(void * pNVMAddress, uint16_t value)
{
	m_itemsSet.push_back(CLLogItem(pNVMAddress, value));
}

void CLLogItemsSet::AddItem(void * pNVMAddress, uint8_t value)
{
	m_itemsSet.push_back(CLLogItem(pNVMAddress, value));
}

void CLLogItemsSet::WriteLogs(CLLogArea & logArea)
{
	for (CLLogItem item : m_itemsSet)
	{
		logArea.WriteLog(item);
	}
}

void CLLogItemsSet::SetValues()
{
	for (CLLogItem item : m_itemsSet)
	{
		item.SetValues();
	}
}

void CLLogItemsSet::ReleaseAllItems()
{
	m_itemsSet.erase(m_itemsSet.begin(), m_itemsSet.end());
}

TRANSACTIONLIB_NS_END