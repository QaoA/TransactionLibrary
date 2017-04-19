#ifndef __TRANSACTION_ABORT_H__
#define __TRANSACTION_ABORT_H__

#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

enum EMAbortReason
{
	NO_VALID_VERSION_IN_READ_TRANSACION,
	OBJECT_OCCUPIED_BY_ANOTHER_WRITE_TRANSACTION,
	UNEXPECTED_ERROR
};

class CLTransactionAbort
{
public:
	CLTransactionAbort(EMAbortReason reason);
	~CLTransactionAbort();

private:
	EMAbortReason m_reason;
};

TRANSACTIONLIB_NS_END
#endif