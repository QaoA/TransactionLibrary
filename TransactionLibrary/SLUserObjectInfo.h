#ifndef __USER_OBJECT_INFO_H__
#define __USER_OBJECT_INFO_H__

#include "TransactionLibraryNameSpace.h"
#include<cstddef>

TRANSACTIONLIB_NS_BEGIN

typedef void(*objectReleaseFunc)(void *);

struct SLUserObjectInfo
{
	size_t m_objectSize;
	objectReleaseFunc m_objectReleaseFunc;
};

TRANSACTIONLIB_NS_END
#endif