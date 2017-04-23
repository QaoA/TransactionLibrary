#include "CLLSAClock.h"

TRANSACTIONLIB_NS_BEGIN

CLLSAClock::CLLSAClock():
m_clock(0)
{
}

CLLSAClock::~CLLSAClock()
{
}

CLLSAClock & CLLSAClock::GetInstance()
{
	static CLLSAClock instance;
	return instance;
}

LSATimeStamp CLLSAClock::Tick()
{
	return ++m_clock;
}

LSATimeStamp CLLSAClock::GetClock()
{
	return m_clock.load();
}

TRANSACTIONLIB_NS_END