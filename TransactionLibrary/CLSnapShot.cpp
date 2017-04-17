#include "CLSnapShot.h"
#include "CLLSAClock.h"
#include "CLBasicData.h"

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