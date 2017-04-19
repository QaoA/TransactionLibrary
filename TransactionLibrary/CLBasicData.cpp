#include "CLBasicData.h"

TRANSACTIONLIB_NS_BEGIN

CLBasicData::CLBasicData()
{
}

CLBasicData::~CLBasicData()
{
}

CLBasicData & CLBasicData::GetInstance()
{
	static CLBasicData instance;
	return instance;
}

CLLSAClock & CLBasicData::GetLSAClock()
{
	return m_clock;
}

TRANSACTIONLIB_NS_END