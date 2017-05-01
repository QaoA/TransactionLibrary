#ifndef __TIME_COUNT_H__
#define __TIME_COUNT_H__

#include <sys/time.h>

class TCLTimeCount
{
public:
	void TimeCountStart();
	void TimeCountEnd();
	long GetDelta();

private:
	timeval m_start;
	timeval m_end;
};

#endif // !__TIME_COUNT_H__
