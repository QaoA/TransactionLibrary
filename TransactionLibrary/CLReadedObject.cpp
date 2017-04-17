#include "CLReadedObject.h"

CLReadedObject::CLReadedObject(CLTransactionalObject * pTransactionalObject, SLObjectVersion * pUserObejctVersion):
m_pTransactionalObject(pTransactionalObject),
m_pUserObjectVersion(pUserObejctVersion)
{
}

CLReadedObject::~CLReadedObject()
{
}