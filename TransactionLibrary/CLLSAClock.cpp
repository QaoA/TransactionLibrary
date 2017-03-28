#include "CLLSAClock.h"
#include "AtomicOperation.h"

CLLSAClock::CLLSAClock():
m_clock(0)
{
}

CLLSAClock::~CLLSAClock()
{
}

LSATimeStamp CLLSAClock::Tick()
{
	return IncreaseAndFetch(&m_clock);
}

LSATimeStamp CLLSAClock::GetClock()
{
	return m_clock;
}