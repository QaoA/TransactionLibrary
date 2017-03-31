#ifndef __TRANSACTION_ABORT_H__
#define __TRANSACTION_ABORT_H__

enum EMAbortReason
{
	NO_VALID_REASON_IN_READ_TRANSACION,
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

#endif
