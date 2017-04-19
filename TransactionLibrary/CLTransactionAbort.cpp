#include "CLTransactionAbort.h"

TRANSACTIONLIB_NS_BEGIN

CLTransactionAbort::CLTransactionAbort(EMAbortReason reason):
m_reason(reason)
{
}

CLTransactionAbort::~CLTransactionAbort()
{
}

TRANSACTIONLIB_NS_END