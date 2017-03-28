#ifndef __TRANSACTIONAL_OBJECT_PROXY_TABLE_H__
#define  __TRANSACTIONAL_OBJECT_PROXY_TABLE_H__

class CLTransactionalObject;

class CLTransanctionalObjectTable
{
private:
	CLTransanctionalObjectTable();
	CLTransanctionalObjectTable(const CLTransanctionalObjectTable &);
	CLTransanctionalObjectTable & operator=(const CLTransanctionalObjectTable &);

public:
	~CLTransanctionalObjectTable();

public:
	static CLTransanctionalObjectTable & GetInstance();

public:
	CLTransactionalObject & Find(CLTransactionalObject & object);
};

#endif