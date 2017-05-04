#ifndef __TEST_LIST_H__
#define __TEST_LIST_H__

#include "NVMUserObject.h"
#include "CLWritePointer.h"
#include "CLReadPointer.h"

using namespace NVMTransaction;

struct TSLList
{
	NVM_OBJECT(TSLList)
public:
	TSLList * m_pPrevious;
	TSLList * m_pNext;
	int m_data;

public:
	static void Init(CLWritePointer<TSLList> pList, int data);
	static void Append(CLWritePointer<TSLList> pList, CLWritePointer<TSLList> pPrevious);
	static void Remove(CLWritePointer<TSLList> pPrevious);
	static void Show(CLReadPointer<TSLList> pList);
};

#endif