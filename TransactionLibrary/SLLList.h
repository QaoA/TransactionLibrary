#ifndef __LIST_H__
#define __LIST_H__

#include "CLTransactionalObject.h"

class SLList
{
public:
	SLList();
	~SLList();

public:
	void Append(CLTransactionalObject<SLList> * pPrevious);
	void Remove();
public:
	CLTransactionalObject<SLList> * m_pNext;
	CLTransactionalObject<SLList> * m_pPrevious;
};

#endif