#ifndef __TEST_LIST_H__
#define __TEST_LIST_H__

#include "NVMUserObject.h"

using namespace NVMTransaction;

struct TSLList
{
	NVM_OBJECT(TSLList)
public:
	TSLList * m_pPrevious;
	TSLList * m_pNext;
	int m_data;

public:
	static void Init(TSLList * pList, int data);
	static void Append(TSLList * pList, TSLList * pPrevious);
	static void Show(TSLList * pList);
	static TSLList * MakeList();
};

#endif