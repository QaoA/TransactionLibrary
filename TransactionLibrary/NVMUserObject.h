#ifndef __NVM_USER_OBJECT_H__
#define __NVM_USER_OBJECT_H__

#include "SLUserObjectInfo.h"
#include "CLWritePointer.h"
#include "NVMMalloc.h"
#include <cstddef>
#include <new>

#define NVM_OBJECT(ObjectType) \
public:\
static void * operator new(size_t size) throw(std::bad_alloc)\
{\
	void * ptr = NVMMalloc::MallocOnNVM(size);\
	if(ptr == nullptr)\
	{\
		throw std::bad_alloc();\
	}\
}\
static void operator delete(void * ptr,size_t size) throw()\
{\
	if(ptr == nullptr)\
	{\
		return;\
	}\
	NVMMalloc::FreeOnNVM(ptr);\
}\
static void ReleaseObject(void * pObject)\
{\
	delete (ObjectType *)(pObject);\
}\
static NVMTransaction::SLUserObjectInfo * GetUserObjectInfo()\
{\
	static NVMTransaction::SLUserObjectInfo userInfo = \
		{\
			.m_objectSize = sizeof(ObjectType),\
			.m_objectReleaseFunc = ReleaseObject\
		};\
	return &userInfo;\
}\
private:\

#endif

