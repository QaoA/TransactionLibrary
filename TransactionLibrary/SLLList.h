#ifndef __LIST_H__
#define __LIST_H__

#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

class SLList
{
public:
	SLList();
	~SLList();

public:
	void Append(SLList * pPrevious);
	void Remove();
	static void Find(SLList * start, int value);

public:
	SLList * m_pNext;
	SLList * m_pPrevious;
	int m_value;
};

TRANSACTIONLIB_NS_END
#endif