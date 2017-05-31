#include <iostream>
#include "ListTestFuncs.h"
#include "TestFuncs.h"
#include "TCLTimeCount.h"
#include <pthread.h>
#include <unistd.h>
#include "CLMutex.h"

using namespace std;
using namespace NVMTransaction;

TSLList * head;

void PreFunc(void *)
{
	CLWritePointer<TSLList> pHead =  CLWritePointer<TSLList>::MakeNewPointer(new TSLList());
	TSLList::Init(pHead,0);
	head = pHead.Get();
}

void append(void * arg)
{
	CLWritePointer<TSLList> pHead(head);
	unsigned long data = (unsigned long)arg;
	CLWritePointer<TSLList> newNode = CLWritePointer<TSLList>::MakeNewPointer(new TSLList());
	newNode->m_data = data;
	TSLList::Append(newNode, pHead);
}

void Find(void * arg)
{
	//CLReadPointer<TSLList> header(head);
	//TSLList::Show(header);
	TSLList::Show(CLReadPointer<TSLList>(head));
}

void * ReadThreadFunc(void *)
{
	RunReadTransaction(Find, nullptr);
}

void * WriteThreadFunc(void *)
{
	cout << "thread " << pthread_self() << " : append begin" << endl;
	for (int i = 0; i < 1000; ++i)
	{
		while (!RunWriteTransaction(append, (void *)i));
	}
	cout << "thread " << pthread_self() << " : append end" << endl;
}

//TSLNormalList * headPure;
//CLMutex m_lock;
//
//void PreFuncPure(void *)
//{
//	headPure = new TSLNormalList();
//	TSLNormalList::Init(headPure, 0);
//}
//
//void appendPure(void * arg)
//{
//	unsigned long data = (unsigned long)(arg);
//	TSLNormalList * pNewNode = new TSLNormalList();
//	pNewNode->m_data = data;
//	m_lock.Lock();
//	TSLNormalList::Append(pNewNode, headPure);
//	m_lock.Unlock();
//}
//
//void FindPure(void * arg)
//{
//	m_lock.Lock();
//	TSLNormalList::Show(headPure);
//	m_lock.Unlock();
//}
//
//void * ReadThreadFunc(void *)
//{
//	FindPure(nullptr);
//}
//
//void * WriteThreadFunc(void *)
//{
//	int i = 0;
//	for (i = 0; i < 1000; ++i)
//	{
//		appendPure((void *)i);
//	}
//}

#define THREAD_NUM 4
#define THREAD_WRITE_NUM 4

int main()
{	
	pthread_t threads[THREAD_NUM];
	RunWriteTransaction(PreFunc, nullptr);
//	TCLTimeCount timeCounter;
//	timeCounter.TimeCountStart();

	for (int i = 0; i < THREAD_WRITE_NUM; ++i)
	{
		pthread_create(&threads[i], NULL, WriteThreadFunc, NULL);
	}
	
	for (int i = THREAD_WRITE_NUM; i < THREAD_NUM; ++i)
	{
		pthread_create(&threads[i], NULL, ReadThreadFunc, NULL);
	}
	
	for (int i = 0; i < THREAD_NUM; ++i)
	{
		pthread_join(threads[i], NULL);
	}
	
	RunWriteTransaction(Find, nullptr);

//	timeCounter.TimeCountEnd();
//	cout << timeCounter.GetDelta() << endl;

    return 0;
}