#include "CLReadedObject.h"

TRANSACTIONLIB_NS_BEGIN

CLReadedObject::CLReadedObject(CLTransactionalObject * pTransactionalObject, SLObjectVersion * pUserObejctVersion):
m_pTransactionalObject(pTransactionalObject),
m_pUserObjectVersion(pUserObejctVersion)
{
}

CLReadedObject::~CLReadedObject()
{
}

TRANSACTIONLIB_NS_END