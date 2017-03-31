#ifndef __READ_POINTER_H__
#define __READ_POINTER_H__

#include "CLTransactionalObject.h"
#include "CLThreadTransactionManager.h"

template<typename T>
class CLReadPointer
{
public:
	CLReadPointer(const T * pUserObject);
	~CLReadPointer();

protected:
	union m_pTransactionalObject
	{
		CLTransactionalObject * m_pTransactionalObject;
		const CLTransactionalObject * m_cpTransactionalObject;
	};
	union m_pUserObject
	{
		void * m_pObject;
		const void * m_cpObject;
	};
};

template<typename T>
inline CLReadPointer<T>::CLReadPointer(const T * pUserObject):
m_pTransactionalObject()
{
	CLTransactionalObject::OpenATransactionalObject(pUserObject, CLThreadTransactionManager::GetWriteTransaction(), T::GetUserInfo());
}

#endif