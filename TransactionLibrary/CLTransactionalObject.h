#ifndef __TRANSACTIONAL_OBJECT_H__
#define __TRANSACTIONAL_OBJECT_H__

class CLLocator;

class CLTransactionalObject
{
public:
	CLTransactionalObject();
	~CLTransactionalObject();

private:
	CLLocator * m_object;
};


#endif