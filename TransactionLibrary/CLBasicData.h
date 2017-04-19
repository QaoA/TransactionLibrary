#ifndef __BASIC_DATA_H__
#define __BASIC_DATA_H__

#include "TransactionLibraryNameSpace.h"
#include "CLLSAClock.h"

TRANSACTIONLIB_NS_BEGIN

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

TRANSACTIONLIB_NS_END
#endif