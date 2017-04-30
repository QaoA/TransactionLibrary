#include "SLNVMNameAddressPair.h"
#include "CLNameServer.h"
#include "SLUserObjectInfo.h"
#include <cstring>
#include <new>

TRANSACTIONLIB_NS_BEGIN

void SLNVMNameAddressPair::Set(char * strName, void * address)
{
	memcpy(m_strName, strName, NAME_MAX_SIZE);
	m_pAddress = address;
}

TRANSACTIONLIB_NS_END