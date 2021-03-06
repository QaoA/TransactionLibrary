#ifndef __LSA_TIME_STAMP_H__
#define __LSA_TIME_STAMP_H__

#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

typedef unsigned long LSATimeStamp ;

#define  LSA_TIME_STAMP_INFINITE (LSATimeStamp)(~0)
#define  LSA_TIME_STAMP_INVALID (LSATimeStamp)(~0)
#define  LSA_TIME_STAMP_START (LSATimeStamp)(0)

TRANSACTIONLIB_NS_END

#endif
