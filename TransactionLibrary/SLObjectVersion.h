#ifndef __OBJECT_VERSION_H__
#define __OBJECT_VERSION_H__

#include "LSATimeStamp.h"

struct SLObjectVersion
{
public:
	SLObjectVersion(void * pUserObject, LSATimeStamp commitTime, SLObjectVersion * pNextVersion);
	~SLObjectVersion();

public:
	void * m_pUserObject;
	LSATimeStamp m_commitTime;
	LSATimeStamp m_validUpperTime;
	SLObjectVersion * m_pNextVersion;
};

#endif