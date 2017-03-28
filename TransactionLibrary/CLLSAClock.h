#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "LSATimeStamp.h"

class CLLSAClock
{
public:
	CLLSAClock();
	~CLLSAClock();

public:
	LSATimeStamp Tick();
	LSATimeStamp GetClock();

private:
	volatile LSATimeStamp m_clock;
};

#endif