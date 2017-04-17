#ifndef __BASIC_DATA_H__
#define __BASIC_DATA_H__

#include "CLLSAClock.h"

class CLBasicData
{
private:
	CLBasicData();
	CLBasicData(const CLBasicData &);
	CLBasicData & operator=(const CLBasicData &);

public:
	~CLBasicData();

public:
	static CLBasicData & GetInstance();

public:
	CLLSAClock & GetLSAClock();

private:
	CLLSAClock m_clock;
};

#endif