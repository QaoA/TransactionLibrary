#ifndef __TEST_SLTList_H__
#define __TEST_SLTList_H__

#include "NVMUserObject.h"

using namespace NVMTransaction;

struct SLTList
{
	NVM_OBJECT(SLTList)
public:
	SLTList(int data);
	SLTList * m_pPrevious;
	SLTList * m_pNext;
	int m_data;

public:
	//void Append(SLTList * pList);

public:
	static void MakeList(void *);
	static void ShowList(SLTList * pList);
	static void ShowListReadOnly(void * pList);
	static void IncreaseAll(void * arg);
	static void Append(CLWritePointer<SLTList> pPrevious, CLWritePointer<SLTList> pNext);
};

#endif
