#ifndef __TRANSACTION_ABORT_H__
#define __TRANSACTION_ABORT_H__

enum EMAbortReason
{
	NO_VALID_REASON_IN_READ_TRANSACION
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
