#ifndef __NVM_NAME_ADDRESS_PAIR_H__
#define __NVM_NAME_ADDRESS_PAIR_H__

#include "NVMMalloc.h"
#include "TransactionLibraryNameSpace.h"
#include "SLUserObjectInfo.h"
#include <cstddef>

TRANSACTIONLIB_NS_BEGIN
	
#define NAME_MAX_SIZE 24

struct SLNVMNameAddressPair
{
public:
	void Set(char * strName, void * address);
	
public:
	char m_strName[NAME_MAX_SIZE];
	void * m_pAddress;
};

TRANSACTIONLIB_NS_END
#endif