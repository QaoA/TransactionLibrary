#ifndef __NVM_USER_OBJECT_H__
#define __NVM_USER_OBJECT_H__

#include "SLUserObjectInfo.h"
#include "CLWritePointer.h"
#include "NVMMalloc.h"
#include <stddef.h>
#include <new>

#define NVM_OBJECT \
public:\
static SLUserObjectInfo * GetUserObjectInfo()\
{\
	static SLUserObjectInfo userInfo = \
		{\
			.m_objectSize = sizeof(*this);\		//这里有问题
		}\
	return &userInfo;\
}\
static void * operator new(std::size_t size) throw(std::bad_alloc)\
{\
	void * ptr = MallocOnNVM(size);\
	if(ptr == nullptr)\
	{\
		throw std::bad_alloc();\
	}\
}\
static void operator delete(void * ptr,std::size size) throw()\
{\
	if(ptr == nullptr)\
	{\
		return;\
	}\
	FreeOnNVM(ptr);\
}\

#define NVM_NEW() \
//T * pt = new T(xxx);
//CLWritePointer<T> ptrName(pt);

#define NVM_DELETE\


#endif

