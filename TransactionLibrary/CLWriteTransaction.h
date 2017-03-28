#ifndef __WRITE_TRANSACTION_H__
#define __WRITE_TRANSACTION_H__

#include "CLTransaction.h"

class CLWriteTransaction : CLTransaction
{
public:
	CLWriteTransaction();
	virtual ~CLWriteTransaction();
};

#endif