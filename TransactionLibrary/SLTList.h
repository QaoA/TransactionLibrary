#ifndef __TEST_SLTList_H__
#define __TEST_SLTList_H__

#include "NVMUserObject.h"

struct SLTList
{
	NVM_OBJECT(SLTList)
public:
	SLTList(int data);
	SLTList * m_pPrevious;
	SLTList * m_pNext;
	int m_data;

public:
	void Append(SLTList * pList);

public:
	static SLTList * MakeList(const int min, const int max);
	static void ShowList(SLTList * pList);
	static void ShowListReadOnly(SLTList * pList);
};

#endif
