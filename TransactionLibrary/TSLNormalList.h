#ifndef __NORMAL_LIST_H__
#define __NORMAL_LIST_H__

#include "NVMUserObject.h"

struct TSLNormalList
{
	NVM_OBJECT(TSLNormalList)
public:
	TSLNormalList * m_pPrevious;
	TSLNormalList * m_pNext;
	int m_data;

public:
	static void Init(TSLNormalList * pList, int data);
	static void Append(TSLNormalList * pList, TSLNormalList * pPrevious);
	static void Show(TSLNormalList * pList);
	static void Remove(TSLNormalList * pList);
};

#endif