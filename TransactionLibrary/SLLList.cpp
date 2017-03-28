#include "SLLList.h"

SLList::SLList():
m_pPrevious(nullptr),
m_pNext(nullptr)
{
}

SLList::~SLList()
{
}

void SLList::Append(CLTransactionalObject<SLList> * previous)
{
	//m_pPrevious = previous;
	//m_pNext = previous->m_pNext;
	//m_pPrevious->m_pNext = this;
	//m_pNext->m_pPrevious = this;
}

void SLList::Remove()
{
	//m_pPrevious->m_pNext = m_pNext;
	//m_pNext->m_pPrevious = m_pPrevious;
}
