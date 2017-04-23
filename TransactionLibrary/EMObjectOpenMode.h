#ifndef __OBJECT_OPEN_MODE_H__
#define __OBJECT_OPEN_MODE_H__

#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

typedef unsigned int EMObjectOpenMode;

#define OPEN_NONE (EMObjectOpenMode)(0x0)
#define OPEN_READ_ONLY (EMObjectOpenMode)(0x1)
#define OPEN_READ (EMObjectOpenMode)(0x2)
#define OPEN_WRITE (EMObjectOpenMode)(0x4)
#define OPEN_NEW (EMObjectOpenMode)(0x8)
#define OPEN_DELETE (EMObjectOpenMode)(0x10)
#define OPEN_CLEAR_MASK (OPEN_DELETE)

TRANSACTIONLIB_NS_END
#endif
