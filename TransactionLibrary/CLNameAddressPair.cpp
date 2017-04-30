#include "CLNameAddressPair.h"
#include "CLNameServer.h"
#include <cstring>

TRANSACTIONLIB_NS_BEGIN

CLNameAddressPair::CLNameAddressPair(SLNVMNameAddressPair * pNVMPair):
m_pNVMPair(pNVMPair)
{
	memcpy(&m_pairCopy, pNVMPair, sizeof(SLNVMNameAddressPair));
}

CLNameAddressPair::~CLNameAddressPair()
{
}

void CLNameAddressPair::Set(char * strName, void * address)
{
	m_pairCopy.Set(strName, address);
}

void CLNameAddressPair::Commit(CLLogItemsSet & logItemSet)
{
	logItemSet.AddItem(m_pNVMPair, sizeof(SLNVMNameAddressPair), &m_pairCopy);
}

void CLNameAddressPair::Abort()
{
}

void CLNameAddressPair::Close()
{
	CLNameServer::GetInstance().Free(m_pNVMPair);
}

TRANSACTIONLIB_NS_END