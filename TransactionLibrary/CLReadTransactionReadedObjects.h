#ifndef __READ_TRANSACTION_READED_OBJECTS_H__
#define __READ_TRANSACTION_READED_OBJECTS_H__

#include "LSATimeStamp.h"
#include "TransactionLibraryNameSpace.h"
#include <list>

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

private:
	std::list<CLReadedObject*> m_readSet;
	bool m_bIsClosed;
	LSATimeStamp m_minValidUpper;
};

#endif