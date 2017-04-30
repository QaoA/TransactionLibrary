#ifndef __NAME_ADDRESS_PAIR_H__
#define __NAME_ADDRESS_PAIR_H__

#include "TransactionLibraryNameSpace.h"
#include "SLNVMNameAddressPair.h"
#include "CLLogItemsSet.h"

TRANSACTIONLIB_NS_BEGIN

class CLNameAddressPair
{
public:
	CLNameAddressPair(SLNVMNameAddressPair * pNVMPair);
	~CLNameAddressPair();
	
public:
	inline char * GetName();
	inline void * GetAddress();
	
public:
	void Set(char * strName, void * address);
	void Commit(CLLogItemsSet & logItemSet);
	void Abort();
	void Close();
	
private:
	SLNVMNameAddressPair * m_pNVMPair;
	SLNVMNameAddressPair m_pairCopy;
};

inline char * CLNameAddressPair::GetName()
{
	return m_pairCopy.m_strName;
}

inline void * CLNameAddressPair::GetAddress()
{
	return m_pairCopy.m_pAddress;
}

TRANSACTIONLIB_NS_END
#endif 