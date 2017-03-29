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