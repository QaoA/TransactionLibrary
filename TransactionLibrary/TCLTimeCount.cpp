#include "TCLTimeCount.h"

void TCLTimeCount::TimeCountStart()
{
	gettimeofday(&m_start, 0);
}

void TCLTimeCount::TimeCountEnd()
{
	gettimeofday(&m_end, 0);
}

long TCLTimeCount::GetDelta()
{
	return (m_end.tv_sec - m_start.tv_sec) * 1000000 + (m_end.tv_usec - m_start.tv_usec);
}
