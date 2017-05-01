#include "TSLNormalList.h"
#include <iostream>

using namespace std;

void TSLNormalList::Init(TSLNormalList * pList, int data)
{
	pList->m_data = data;
	pList->m_pNext = pList;
	pList->m_pPrevious = pList;
}

void TSLNormalList::Append(TSLNormalList * pList, TSLNormalList * pPrevious)
{
	TSLNormalList * pNext = pPrevious->m_pNext;
	pList->m_pNext = pNext;
	pList->m_pPrevious = pPrevious;
	pPrevious->m_pNext = pList;
	pNext->m_pPrevious = pList;
}

void TSLNormalList::Show(TSLNormalList * pList)
{
	TSLNormalList * pHead = pList;
	TSLNormalList * pTmp = pHead;
	do
	{
		//cout << pTmp->m_data << "\t";
		pTmp = pTmp->m_pNext;
	} while (pTmp != pList);
	//cout << endl;
}

TSLNormalList * TSLNormalList::MakeList()
{
	TSLNormalList * pHead = new TSLNormalList;
	TSLNormalList::Init(pHead, 0);
	for (int i = 1; i < 1000; ++i)
	{
		TSLNormalList * pTmp = new TSLNormalList;
		TSLNormalList::Init(pTmp, i);
		TSLNormalList::Append(pTmp, pHead);
	}
	return pHead;
}
