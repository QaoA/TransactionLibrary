#ifndef __OBJECT_VERSION_H__
#define __OBJECT_VERSION_H__

#include "TransactionLibraryNameSpace.h"
#include "LSATimeStamp.h"

TRANSACTIONLIB_NS_BEGIN

class CLTransactionalObject;

struct SLObjectVersion
{
public:
	SLObjectVersion(void * pUserObject, LSATimeStamp commitTime, SLObjectVersion * pNextVersion, CLTransactionalObject * pOwnerObject);
	~SLObjectVersion();

public:
	void * m_pUserObject;
	LSATimeStamp m_commitTime;
	LSATimeStamp m_validUpperTime;
	SLObjectVersion * m_pNextVersion;
	CLTransactionalObject * m_pOwnerObject;
};

TRANSACTIONLIB_NS_END
#endif