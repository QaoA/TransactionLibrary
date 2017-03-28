#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__

enum TransactionStatus
{
	ACTIVE,
	ABORTED,
	COMMITTED,
};

class CLTransaction
{
public:
	CLTransaction();
	virtual ~CLTransaction();

public:
	inline TransactionStatus GetStatus();

private:
	TransactionStatus m_status;
};

inline TransactionStatus CLTransaction::GetStatus()
{
	return m_status;
}

#endif