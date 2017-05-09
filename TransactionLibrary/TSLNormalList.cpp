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
	int i = 0;
	do
	{
		//cout << pTmp->m_data << "\t";
		pTmp = pTmp->m_pNext;
		i += pTmp->m_data;
	} while (pTmp != pList);
	//cout << endl;
	//cout << "i = " << i << endl;
}

void TSLNormalList::Remove(TSLNormalList * pPrevious)
{
	TSLNormalList * pList = pPrevious->m_pNext;
	TSLNormalList * pNext = pList->m_pNext;
	pPrevious->m_pNext = pNext;
	pNext->m_pPrevious = pPrevious;
	delete pList;
}
