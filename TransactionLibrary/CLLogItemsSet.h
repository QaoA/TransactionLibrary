#ifndef __LOG_ITEMS_SET_H__
#define __LOG_ITEMS_SET_H__

#include <list>
#include "CLLogArea.h"
#include "CLLogItem.h"
#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

using NVMMalloc::CLLogItem;
using NVMMalloc::CLLogArea;

class CLLogItemsSet
{
public:
	CLLogItemsSet();
	~CLLogItemsSet();

public:
	void Reset();
	void AddItem(void * pNVMAddress, unsigned long size, void * pValue);
	void AddItem(void * pNVMAddress, uint64_t value);
	void AddItem(void * pNVMAddress, uint32_t value);
	void AddItem(void * pNVMAddress, uint16_t value);
	void AddItem(void * pNVMAddress, uint8_t value);
	void WriteLogs(CLLogArea & logArea);
	void SetValues();
	void ReleaseAllItems();

private:
	std::list<CLLogItem> m_itemsSet;
};

TRANSACTIONLIB_NS_END
#endif