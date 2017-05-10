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
	for (int i = 0; i < 1000; ++i)
	{
		while (!RunWriteTransaction(append, (void *)i));
	}
}

#define THREAD_NUM 4

int main()
{
	pthread_t threads[THREAD_NUM];
	RunWriteTransaction(PreFunc, nullptr);
	TCLTimeCount timeCounter;
	timeCounter.TimeCountStart();

	for (int i = 0; i < THREAD_NUM; ++i)
	{
		if (i == THREAD_NUM -1)
		{
			pthread_create(&threads[i], NULL, ReadThreadFunc, NULL);
		}
		else
		{
			pthread_create(&threads[i], NULL, WriteThreadFunc, NULL);
		}
	}

	for (int i = 0; i < THREAD_NUM; ++i)
	{
		pthread_join(threads[i], NULL);
	}

	timeCounter.TimeCountEnd();
	cout << timeCounter.GetDelta() << endl;
	ReadThreadFunc(nullptr);

    return 0;
}