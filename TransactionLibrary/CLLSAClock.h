#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "LSATimeStamp.h"
#include <atomic>

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

#endif