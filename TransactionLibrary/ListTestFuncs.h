#ifndef __LIST_TEST_FUNCS_H__
#define __LIST_TEST_FUNCS_H__

#include "TSLList.h"
#include "TSLNormalList.h"

#define LIST_LENGTH 100

CLWritePointer<TSLList> MakeListNVM()
{
	CLWritePointer<TSLList> head = CLWritePointer<TSLList>::MakeNewPointer(new TSLList);
	TSLList::Init(head, -1);
	for (int i = 0; i < LIST_LENGTH; ++i)
	{
		CLWritePointer<TSLList> newNode = CLWritePointer<TSLList>::MakeNewPointer(new TSLList);
		newNode->m_data = i;
		TSLList::Append(newNode, head);
	}
	return head;
}

void ReleaseListNVM(CLWritePointer<TSLList> pHead)
{
	for (int i = 0; i < LIST_LENGTH; ++i)
	{
		TSLList::Remove(pHead);
	}
	TSLList::Remove(pHead);
}

TSLNormalList * MakeListNormal()
{
	TSLNormalList * head = new TSLNormalList;
	TSLNormalList::Init(head, -1);
	for (int i = 0; i < 100; ++i)
	{
		TSLNormalList * newNode = new TSLNormalList;
		newNode->m_data = i;
		TSLNormalList::Append(newNode, head);
	}
}

void ReleaseListNormal(TSLNormalList * head)
{
	for (int i = 0; i < LIST_LENGTH; ++i)
	{
		TSLNormalList::Remove(head);
	}
	TSLNormalList::Remove(head);
}

#endif