#include "CLSnapShot.h"
#include "CLLSAClock.h"
#include "CLBasicData.h"

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
	m_lower = CLBasicData::GetInstance().GetLSAClock().GetClock();
	m_upper = LSA_TIME_STAMP_INFINITE;
}

TRANSACTIONLIB_NS_END