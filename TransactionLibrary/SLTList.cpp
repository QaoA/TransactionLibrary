#include "SLTList.h"
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

void SLTList::Append(SLTList * pList)
{
	pList->m_pNext = m_pNext;
	pList->m_pPrevious = this;
	m_pNext->m_pPrevious = pList;
	m_pNext = pList;
}

SLTList * SLTList::MakeList(const int min, const int max)
{
	SLTList * pList;
	for (int i = min; i < max; ++i)
	{
		if (i == min)
		{
			pList = new SLTList(i);
		}
		else
		{
			SLTList * pTmpList = new SLTList(i);
			NVMMalloc::NotifyNVMMemoryGet(pTmpList);
			pList->Append(pTmpList);
		}
	}
	return pList;
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

void SLTList::ShowListReadOnly(SLTList * pList)
{
	//SLTList * pHead = pList;
	//SLTList * pTmp = pHead;
	//do
	//{
	//	cout << pTmp->m_data << "\t" << endl; _
	//		pTmp = pTmp->m_pNext;
	//} while (pTmp != pHead);
	CLReadOnlyPointer<SLTList> pHead(pList);
	CLReadOnlyPointer<SLTList> pTmp = pHead;
	do 
	{
		cout << pTmp->m_data << "\t" << endl;
		pTmp = pTmp->m_pNext;
	} while (!(pTmp == pHead));
}
