#ifndef __BASIC_FUNCTIONS_H__
#define __BASIC_FUNCTIONS_H__

#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

template<typename T>
inline const T Min(const T & first, const T & second)
{
	return first < second ? first : second;
}

template<typename T>
inline const T Max(const T & first, const T & second)
{
	return first < second ? second : first;
}

TRANSACTIONLIB_NS_END
#endif