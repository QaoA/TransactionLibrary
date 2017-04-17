#include "SLTList.h"

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
			pList->Append(pTmpList);
		}
	}
}