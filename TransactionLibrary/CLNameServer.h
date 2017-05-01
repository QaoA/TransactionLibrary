#ifndef __NAME_SERVER_H__
#define __NAME_SERVER_H__

#include "CLNameServerArea.h"
#include "CLSpinLock.h"
#include "TransactionLibraryNameSpace.h"
#include <list>

TRANSACTIONLIB_NS_BEGIN

class SLNVMNameAddressPair;

class CLNameServer
{
private:
	CLNameServer(NVMMalloc::CLNameServerArea * pArea);
	CLNameServer(const CLNameServer &);
	CLNameServer & operator=(const CLNameServer &);

public:
	~CLNameServer();

public:
	static CLNameServer & GetInstance();
	
public:
	SLNVMNameAddressPair * Get();
	void Free(SLNVMNameAddressPair * pObject);
	SLNVMNameAddressPair * Find(char * strName);
	SLNVMNameAddressPair * FindAndGet(char * strName);

private:
	NVMMalloc::CLNameServerArea * m_pArea;
	CLSpinLock m_lock;
	std::list<unsigned int> m_availableObjectsIndex;
	std::list<unsigned int> m_objectsLookupList;
};

TRANSACTIONLIB_NS_END
#endif