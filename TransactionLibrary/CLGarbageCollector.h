#ifndef __GABAGE_COLLECTOR_H__
#define __GABAGE_COLLECTOR_H__

typedef void(*DestructFunc)(void *);

class CLGarbageCollector
{
public:
	CLGarbageCollector();
	~CLGarbageCollector();

public:
	static CLGarbageCollector & GetInstance();

public:
	void CollectGarbage(void * gabage, DestructFunc releaseFunc);
};

#endif