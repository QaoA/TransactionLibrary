#ifndef __READED_OBJECT_H__
#define __READED_OBJECT_H__

#include "LSATimeStamp.h"
#include "SLObjectVersion.h"
#include "CLTransactionalObject.h"
#include "TransactionLibraryNameSpace.h"

class CLObject;
class CLTransactionalObject;

class CLReadedObject
{
public:
	CLReadedObject(CLTransactionalObject * pTransactionalObject, SLObjectVersion * pUserObejctVersion);
	~CLReadedObject();

public:
	inline void * GetUserObject();
	inline LSATimeStamp GetUpperTime();
	inline CLTransactionalObject * GetTransactionalObject();

private:
	CLTransactionalObject * m_pTransactionalObject;
	SLObjectVersion * m_pUserObjectVersion;
};

inline void * CLReadedObject::GetUserObject()
{
	return m_pUserObjectVersion->m_pUserObject;
}

inline LSATimeStamp CLReadedObject::GetUpperTime()
{
	return m_pUserObjectVersion->m_validUpperTime;
}

inline CLTransactionalObject * CLReadedObject::GetTransactionalObject()
{
	return m_pTransactionalObject;
}

#endif