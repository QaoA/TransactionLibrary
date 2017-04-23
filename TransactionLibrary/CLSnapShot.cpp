#include "CLSnapShot.h"
#include "CLLSAClock.h"

TRANSACTIONLIB_NS_BEGIN

CLSnapShot::CLSnapShot()
{
	Reset();
}

CLSnapShot::~CLSnapShot()
{
}

void CLSnapShot::Reset()
{
	m_lower = CLLSAClock::GetInstance().GetClock();
	m_upper = LSA_TIME_STAMP_INFINITE;
}

TRANSACTIONLIB_NS_END