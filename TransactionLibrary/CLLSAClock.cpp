#include "CLLSAClock.h"

CLLSAClock::CLLSAClock():
m_clock(0)
{
}

CLLSAClock::~CLLSAClock()
{
}

LSATimeStamp CLLSAClock::Tick()
{
	return ++m_clock;
}

LSATimeStamp CLLSAClock::GetClock()
{
	return m_clock.load();
}