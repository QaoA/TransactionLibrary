#include "CLReadedObject.h"

CLReadedObject::CLReadedObject(CLTransactionalObject * pTransactionalObject, void * pUserObject, LSATimeStamp lower, LSATimeStamp upper):
m_pTransactionalObject(pTransactionalObject),
m_pUserObject(pUserObject),
m_lower(lower),
m_upper(upper)
{
}

CLReadedObject::~CLReadedObject()
{
}
