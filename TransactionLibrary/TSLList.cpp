#include "TSLList.h"
#include "CLWritePointer.h"
#include <iostream>

using namespace std;

void TSLList::Init(TSLList * pList, int data)
{
	CLWritePointer<TSLList> ThisNode(pList);
	ThisNode->m_data = data;
	ThisNode->m_pNext = pList;
	ThisNode->m_pPrevious = pList;
}

void TSLList::Append(TSLList * pList, TSLList * pPreviousNode)
{
	CLWritePointer<TSLList> ThisNode(pList);
	CLWritePointer<TSLList> pPrevious(pPreviousNode);
	CLWritePointer<TSLList> pNext(pPrevious->m_pNext);
	ThisNode->m_pNext = pNext;
	ThisNode->m_pPrevious = pPrevious;
	pPrevious->m_pNext = pList;
	pNext->m_pPrevious = pList;
}

void TSLList::Show(TSLList * pList)
{
	CLReadPointer<TSLList> pHead(pList);
	CLReadPointer<TSLList> pTmp(pHead);
	do 
	{
		cout << pTmp->m_data << "\t";
		pTmp = pTmp->m_pNext;
	} while (pTmp.Get() != pList);
	cout << endl;
}

TSLList * TSLList::MakeList()
{
	CLWritePointer<TSLList> pHead = CLWritePointer<TSLList>::MakeNewPointer(new TSLList);
	TSLList::Init(pHead, 0);
	for (int i = 1; i < 10; ++i)
	{
		CLWritePointer<TSLList> pTmp = CLWritePointer<TSLList>::MakeNewPointer(new TSLList);
		TSLList::Init(pTmp, i);
		TSLList::Append(pTmp, pHead);
	}
	return pHead;
}
