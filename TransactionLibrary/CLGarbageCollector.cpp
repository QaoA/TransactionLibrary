#include "CLGarbageCollector.h"

TRANSACTIONLIB_NS_BEGIN

CLGarbageCollector::CLGarbageCollector()
{
}

CLGarbageCollector::~CLGarbageCollector()
{
}

CLGarbageCollector & CLGarbageCollector::GetInstance()
{
	static CLGarbageCollector instance;
	return instance;
}

void CLGarbageCollector::CollectGarbage(void * gabage, DestructFunc releaseFunc)
{
}

TRANSACTIONLIB_NS_END