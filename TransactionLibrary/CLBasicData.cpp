#include "CLBasicData.h"

CLBasicData::CLBasicData()
{
}

CLBasicData::~CLBasicData()
{
}

CLBasicData & CLBasicData::GetInstance()
{
	CLBasicData instance;
	return instance;
}

CLLSAClock & CLBasicData::GetLSAClock()
{
	return m_clock;
}