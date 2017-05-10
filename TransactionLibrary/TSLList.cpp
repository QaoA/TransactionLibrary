#include "TSLList.h"
#include <iostream>

using namespace std;

void TSLList::Init(CLWritePointer<TSLList> pList, int data)
{
	pList->m_data = data;
	pList->m_pNext = pList;
	pList->m_pPrevious = pList;
}

void TSLList::Append(CLWritePointer<TSLList> pList, CLWritePointer<TSLList> pPrevious)
{
	CLWritePointer<TSLList> pNext = CLWritePointer<TSLList>(pPrevious->m_pNext);
	pList->m_pPrevious = pPrevious;
	pList->m_pNext = pNext;
	pPrevious->m_pNext = pList;
	pNext->m_pPrevious = pList;
}

void TSLList::Remove(CLWritePointer<TSLList> pPrevious)
{
	CLWritePointer<TSLList> pList = CLWritePointer<TSLList>(pPrevious->m_pNext);
	CLWritePointer<TSLList> pNext = CLWritePointer<TSLList>(pList->m_pNext);
	pPrevious->m_pNext = pNext;
	pNext->m_pPrevious = pPrevious;
	CLWritePointer<TSLList>::DeleteByPointer(pList);
}

void TSLList::Show(CLReadPointer<TSLList> pList)
{
	CLReadPointer<TSLList> pTmp(pList);
	int i = 0;
	do
	{
		//cout << pTmp->m_data << "\t";
		pTmp = pTmp->m_pNext;
		i++;
	} while (pTmp != pList);
	cout << endl;
	cout << "list length : " << i << endl;
}
