#ifndef __READ_TRANSACTION_READED_OBJECTS_H__
#define __READ_TRANSACTION_READED_OBJECTS_H__

#include "LSATimeStamp.h"
#include "TransactionLibraryNameSpace.h"
#include <map>

TRANSACTIONLIB_NS_BEGIN

struct SLObjectVersion;

class CLReadTransactionReadedObjects
{
public:
	CLReadTransactionReadedObjects();
	~CLReadTransactionReadedObjects();

public:
	void Reset();
	void AppendObject(SLObjectVersion * pVersion);
	LSATimeStamp GetMinValidUpper();
	void Commit();
	void Abort();
	SLObjectVersion * FindObject(void * pUserObject);

private:
	std::map<void *, SLObjectVersion*> m_readSet;
	bool m_bIsClosed;
	LSATimeStamp m_minValidUpper;
};

TRANSACTIONLIB_NS_END
#endif