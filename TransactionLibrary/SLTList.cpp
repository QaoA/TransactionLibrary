#include "SLTList.h"
#include "CLWritePointer.h"
#include "CLReadOnlyPointer.h"
#include <iostream>

using namespace std;
using namespace NVMTransaction;

SLTList::SLTList(int data) :
	m_pPrevious(this),
	m_pNext(this),
	m_data(data)
{
}

//void SLTList::Append(SLTList * pList)
//{
//	pList->m_pNext = m_pNext;
//	pList->m_pPrevious = this;
//	m_pNext->m_pPrevious = pList;
//	m_pNext = pList;
//}

void SLTList::MakeList(void *)
{
	//SLTList * pList;
	//for (int i = min; i < max; ++i)
	//{
	//	if (i == min)
	//	{
	//		pList = new SLTList(i);
	//	}
	//	else
	//	{
	//		SLTList * pTmpList = new SLTList(i);
	//		NVMMalloc::NotifyNVMMemoryGet(pTmpList);
	//		pList->Append(pTmpList);
	//	}
	//}
	
	int min = 0;
	int max = 5;
	
	CLWritePointer<SLTList> pList(nullptr);
	for (int i = min; i < max; ++i)
	{
		if (i == min)
		{
			pList = CLWritePointer<SLTList>::MakeNewObjectPointer(new SLTList(i));
		}
		else
		{
			CLWritePointer<SLTList> pTmpList = CLWritePointer<SLTList>::MakeNewObjectPointer(new SLTList(i));
			Append(pList, pTmpList);
		}
	}
}

void SLTList::ShowList(SLTList * pList)
{
	SLTList * pHead = pList;
	SLTList * pTmp = pHead;
	do 
	{
		cout << pTmp->m_data << "\t" << endl;
		pTmp = pTmp->m_pNext;
	} while (pTmp!= pHead);
}

void SLTList::ShowListReadOnly(void * pArg)
{
	//SLTList * pHead = pList;
	//SLTList * pTmp = pHead;
	//do
	//{
	//	cout << pTmp->m_data << "\t" << endl; _
	//		pTmp = pTmp->m_pNext;
	//} while (pTmp != pHead);
	SLTList * pList = (SLTList *)(pArg);
	CLReadOnlyPointer<SLTList> pHead(pList);
	CLReadOnlyPointer<SLTList> pTmp = pHead;
	do 
	{
		cout << pTmp->m_data << "\t" << endl;
		pTmp = pTmp->m_pNext;
	} while (!(pTmp == pHead));
}

void SLTList::IncreaseAll(void * arg)
{
	SLTList * pList = (SLTList *)(arg);
	CLWritePointer<SLTList> pHead(pList);
	CLWritePointer<SLTList> pTmp = pHead;
	do
	{
		cout << pTmp->m_data << " v1\t" << endl;
		pTmp->m_data++;
		cout << pTmp->m_data << " v2\t" << endl;
		pTmp = pTmp->m_pNext;
	} while (pTmp != pHead);
}

void SLTList::Append(CLWritePointer<SLTList> pPrevious, CLWritePointer<SLTList> pNext)
{
	CLWritePointer<SLTList> pOldNext = pPrevious->m_pNext;
	pNext->m_pNext = pOldNext;
	pNext->m_pPrevious = pPrevious;
	pPrevious->m_pNext = pNext;
	if (pOldNext != nullptr)
	{
		pOldNext->m_pPrevious = pNext;
	}
}
