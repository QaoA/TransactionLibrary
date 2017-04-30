#ifndef __NAME_ADDRESS_PAIR_SET_H__
#define __NAME_ADDRESS_PAIR_SET_H__

#include "TransactionLibraryNameSpace.h"
#include <vector>

TRANSACTIONLIB_NS_BEGIN

class CLNameAddressPair;
class CLLogItemsSet;

class CLNameAddressPairSet
{
public:
	CLNameAddressPairSet();
	~CLNameAddressPairSet();

public:
	CLNameAddressPair * FindByName(char * strName);
	void AddObject(CLNameAddressPair * pPair);
	void Clear();
	void Commit(CLLogItemsSet & logItemSet);
	void Abort();
	void Close();

private:
	std::vector<CLNameAddressPair *> m_set;
};

TRANSACTIONLIB_NS_END
#endif