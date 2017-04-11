#ifndef __LOG_ITEMS_SET_H__
#define __LOG_ITEMS_SET_H__

#include <list>

class CLLogItem;
class CLLogArea;

class CLLogItemsSet
{
public:
	CLLogItemsSet();
	~CLLogItemsSet();

public:
	void AddItem(CLLogItem * item);
	void AddItem(void * pNVMAddress, unsigned long size, void * pValue);
	void WriteLogs(CLLogArea & logArea);
	void SetValues();
	void ReleaseAllItems();

private:
	std::list<CLLogItem *> m_itemsSet;
};

#endif