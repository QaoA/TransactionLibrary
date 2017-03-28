#ifndef __TRANSACTIONAL_OBJECT_PROXY_H__
#define __TRANSACTIONAL_OBJECT_PROXY_H__

#include "LSATimeStamp.h"

class CLObject;
class CLTransaction;
class CLTransactionalObject;

#define CACHE_MAX_OLD_VERSIONS_COUNT 3

class CLLocator
{
private:
	struct SLOldVersion
	{
		LSATimeStamp m_tiemStamp;
		void * m_object;
	};

public:
	CLLocator(CLObject & object);
	~CLLocator();

private:
	CLTransaction * m_pOwner;
	CLObject * m_pObject;
	void * m_TentativeVersion;
	LSATimeStamp m_currentVersionStamp;
	SLOldVersion m_oldVersions[CACHE_MAX_OLD_VERSIONS_COUNT];
};

#endif