#ifndef __READ_TRANSACTION_READED_OBJECTS_H__
#define __READ_TRANSACTION_READED_OBJECTS_H__

#include "LSATimeStamp.h"
#include "TransactionLibraryNameSpace.h"
#include <map>

TRANSACTIONLIB_NS_BEGIN

class CLReadedObject;

class CLReadTransactionReadedObjects
{
public:
	CLReadTransactionReadedObjects();
	~CLReadTransactionReadedObjects();

public:
	void Reset();
	void AppendObject(CLReadedObject * pObject);
	LSATimeStamp GetMinValidUpper();
	void Commit();
	void Abort();
	CLReadedObject * FindObject(void * pUserObject);

public:
	static bool CompareReadedObject(CLReadedObject * pObject1, CLReadedObject * pObject2);

private:
	std::map<void *, CLReadedObject*> m_readSet;
	bool m_bIsClosed;
	LSATimeStamp m_minValidUpper;
};

TRANSACTIONLIB_NS_END
#endif