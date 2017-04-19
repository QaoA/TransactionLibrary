#ifndef __READED_OBJECT_H__
#define __READED_OBJECT_H__

#include "LSATimeStamp.h"
#include "SLObjectVersion.h"
#include "CLTransactionalObject.h"
#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

class CLObject;
class CLTransactionalObject;

class CLReadedObject
{
public:
	CLReadedObject(CLTransactionalObject * pTransactionalObject, SLObjectVersion * pUserObejctVersion);
	~CLReadedObject();

public:
	inline void * GetUserObjectCopy();
	inline LSATimeStamp GetUpperTime();
	inline void * GetUserObjectNVMAddress();
	inline CLTransactionalObject * GetTransactionalObject();

private:
	CLTransactionalObject * m_pTransactionalObject;
	SLObjectVersion * m_pUserObjectVersion;
};

inline void * CLReadedObject::GetUserObjectCopy()
{
	return m_pUserObjectVersion->m_pUserObject;
}

inline LSATimeStamp CLReadedObject::GetUpperTime()
{
	return m_pUserObjectVersion->m_validUpperTime;
}

inline void * CLReadedObject::GetUserObjectNVMAddress()
{
	return m_pTransactionalObject->GetUserObjectNVMAddress();
}

inline CLTransactionalObject * CLReadedObject::GetTransactionalObject()
{
	return m_pTransactionalObject;
}

TRANSACTIONLIB_NS_END
#endif