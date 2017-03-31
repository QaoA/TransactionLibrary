#ifndef __NVM_USER_OBJECT_H__
#define __NVM_USER_OBJECT_H__

#include "SLUserObjectInfo.h"

#define NVM_OBJECT \
public:\
static SLUserObjectInfo * GetUserObjectInfo()\
{\
	static SLUserObjectInfo userInfo = \
		{\
			.m_objectSize = sizeof(*this);\
		}\
	return &userInfo;\
}\

#endif

