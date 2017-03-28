#include "CLSnapShot.h"
#include "CLLSAClock.h"
#include "CLBasicData.h"

CLSnapShot::CLSnapShot():
m_lower(CLBasicData::GetInstance().GetLSAClock().GetClock()),
m_upper(LSA_TIME_STAMP_INFINITE)
{
}

CLSnapShot::~CLSnapShot()
{
}
