#ifndef __READED_OBJECT_H__
#define __READED_OBJECT_H__

#include "LSATimeStamp.h"

class CLObject;
class CLTransactionalObject;

class CLReadedObject
{
public:
	CLReadedObject(CLTransactionalObject * pTransactionalObject, void * pUserObject, LSATimeStamp lower, LSATimeStamp upper);
	~CLReadedObject();

private:
	CLTransactionalObject * m_pTransactionalObject;
	void * m_pUserObject;
	LSATimeStamp m_lower;
	LSATimeStamp m_upper;
};

#endif