#ifndef __DELETED_OBJECT_SET_H__
#define __DELETED_OBJECT_SET_H__

#include <ctime>
#include <list>
#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

typedef void(*DestructFunc)(void *);

class CLDeletedObjectSet
{
private:
	struct SLDeletedObject
	{
		void * m_pObject;
		DestructFunc m_releaseFunc;
	};
	
public:
	CLDeletedObjectSet();
	~CLDeletedObjectSet();
	
public:
	void AddObject(void * pObject, DestructFunc releaseFunc);
	void ReleaseAll();
	void SetTimeStamp();
	clock_t GetTimeStamp();
	
private:
	clock_t m_timeStamp;
	std ::list<SLDeletedObject> m_objectSet;
};

TRANSACTIONLIB_NS_END

#endif