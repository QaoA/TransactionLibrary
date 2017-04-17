#include "CLGarbageCollector.h"

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
