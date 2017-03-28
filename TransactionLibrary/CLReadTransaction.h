#ifndef __READ_TRANSACTION_H__
#define __READ_TRANSACTION_H__

#include "CLTransaction.h"

class CLReadTransaction : CLTransaction
{
public:
	CLReadTransaction();
	virtual ~CLReadTransaction();
};

#endif