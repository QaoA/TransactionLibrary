#include "SLLList.h"
#include <cassert>

TRANSACTIONLIB_NS_BEGIN

SLList::SLList():
m_pPrevious(nullptr),
m_pNext(nullptr)
{
}

SLList::~SLList()
{
}

void SLList::Append(SLList * previous)
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

void SLList::Find(SLList * start, int value)
{
	assert(start);
	SLList * current = start;
	while (current != start)
	{
		if (current->m_value == value)
		{
			break;
		}
		current = current->m_pNext;
	}
}

TRANSACTIONLIB_NS_END