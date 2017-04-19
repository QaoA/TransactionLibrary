#ifndef __NVM_USER_OBJECT_H__
#define __NVM_USER_OBJECT_H__

#include "SLUserObjectInfo.h"
#include "CLWritePointer.h"
#include "NVMMalloc.h"
#include <cstddef>
#include <new>

#define NVM_OBJECT(ObjectType) \
public:\
static NVMTransaction::SLUserObjectInfo * GetUserObjectInfo()\
{\
	static NVMTransaction::SLUserObjectInfo userInfo = \
		{\
			.m_objectSize = sizeof(ObjectType)\
		};\
	return &userInfo;\
}\
static void * operator new(std::size_t size) throw(std::bad_alloc)\
{\
	void * ptr = NVMMalloc::MallocOnNVM(size);\
	if(ptr == nullptr)\
	{\
		throw std::bad_alloc();\
	}\
}\
static void operator delete(void * ptr,std::size_t size) throw()\
{\
	if(ptr == nullptr)\
	{\
		return;\
	}\
	NVMMalloc::FreeOnNVM(ptr);\
}\
private:\


//#define NVM_NEW() \
////T * pt = new T(xxx);
////CLWritePointer<T> ptrName(pt);
//
//#define NVM_DELETE\


#endif

