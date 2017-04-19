#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "TransactionLibraryNameSpace.h"
#include "LSATimeStamp.h"
#include <atomic>

TRANSACTIONLIB_NS_BEGIN

class CLLSAClock
{
public:
	CLLSAClock();
	~CLLSAClock();

public:
	LSATimeStamp Tick();
	LSATimeStamp GetClock();

private:
	std::atomic<LSATimeStamp> m_clock;
};

TRANSACTIONLIB_NS_END
#endif