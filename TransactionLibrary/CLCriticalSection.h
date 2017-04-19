#ifndef __CRITICAL_SECTION_H__
#define __CRITICAL_SECTION_H__

#include "ILLock.h"
#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

class CLCriticalSection
{
public:
	CLCriticalSection(ILLock * pLock);
	~CLCriticalSection();

private:
	ILLock * m_pLock;
};

TRANSACTIONLIB_NS_END
#endif