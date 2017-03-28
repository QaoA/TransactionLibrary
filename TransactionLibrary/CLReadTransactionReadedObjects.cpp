#include "CLReadTransactionReadedObjects.h"
#include <cassert>

CLReadTransactionReadedObjects::CLReadTransactionReadedObjects()
{
}

CLReadTransactionReadedObjects::~CLReadTransactionReadedObjects()
{
}

void CLReadTransactionReadedObjects::AppendObject(CLReadedObject * pObject)
{
	assert(pObject);
	m_readSet.insert(pObject);
}
