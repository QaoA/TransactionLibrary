#include "CLReadedObject.h"

CLReadedObject::CLReadedObject(CLTransactionalObject * pTransactionalObject, void * pReadedVersion, LSATimeStamp lower, LSATimeStamp upper):
m_pTransactionalObject(pTransactionalObject),
m_pReadedVersion(pReadedVersion),
m_lower(lower),
m_upper(upper)
{
}

CLReadedObject::~CLReadedObject()
{
}
