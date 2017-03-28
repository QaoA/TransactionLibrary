#ifndef __LSA_READ_ANALYSISER_H__
#define __LSA_READ_ANALYSISER_H__

class CLSnapShot;
class CLReadedObject;
class CLReadTransactionReadedObjects;
class CLTransactionalObject;

class CLLSAReadAnlysiser
{
public:
	CLLSAReadAnlysiser();
	~CLLSAReadAnlysiser();

public:
	CLReadedObject * DoAnalysis(CLSnapShot & snapshot, CLReadTransactionReadedObjects & readSet, CLTransactionalObject & object);
};

#endif