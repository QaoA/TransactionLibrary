#ifndef __READ_TRANSACTION_READED_OBJECTS_H__
#define __READ_TRANSACTION_READED_OBJECTS_H__

#include "LSATimeStamp.h"
#include <set>

class CLReadedObject;

class CLReadTransactionReadedObjects
{
public:
	CLReadTransactionReadedObjects();
	~CLReadTransactionReadedObjects();

public:
	void AppendObject(CLReadedObject * pObject);
	LSATimeStamp GetMinValidUpper();
private:
	std::set<CLReadedObject*> m_readSet;
};

#endif