#include "SLObjectVersion.h"

TRANSACTIONLIB_NS_BEGIN

SLObjectVersion::SLObjectVersion(void * pUserObject, LSATimeStamp commitTime, SLObjectVersion * pNextVersion, CLTransactionalObject * pOwnerObject):
m_pUserObject(pUserObject),
m_commitTime(commitTime),
m_validUpperTime(LSA_TIME_STAMP_INFINITE),
m_pNextVersion(pNextVersion),
m_pOwnerObject(pOwnerObject)
{
}

SLObjectVersion::~SLObjectVersion()
{
}

TRANSACTIONLIB_NS_END