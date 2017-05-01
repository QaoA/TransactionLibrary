#include <iostream>
#include "CLTransactionAPIs.h"
#include "TSLList.h"
#include "TSLNormalList.h"
#include "NVMMalloc.h"
#include "TCLTimeCount.h"

using namespace std;
using namespace NVMTransaction;

void DoFunc(void *)
{
	TSLList * pHead = TSLList::MakeList();
	CLTransactionAPIs::SetAddressByName("listHead", pHead);
	//TSLList::Show(pHead);
}

void DoFuncNormal(void *)
{
	TSLNormalList * pHead = TSLNormalList::MakeList();
}

void DoFunc2(void *)
{
	TSLList * pHead = (TSLList *)CLTransactionAPIs::GetAddressByName("listHead");
	TSLList::Show(pHead);
}

void DoFunc3(void *)
{
	TSLList * pHead = (TSLList *)CLTransactionAPIs::GetAddressByName("listHead");
	TSLList::Show(pHead);
}

void DoFunc3Normal(void * pHead)
{
	TSLNormalList::Show((TSLNormalList *)pHead);
}

int main()
{
	TSLNormalList * pHead = TSLNormalList::MakeList();
	TCLTimeCount timeCounter;
	timeCounter.TimeCountStart();
	CLTransactionAPIs::RunReadTransaction(DoFunc3Normal, pHead);
	timeCounter.TimeCountEnd();
	cout << timeCounter.GetDelta() << endl;
	//CLTransactionAPIs::RunReadTransaction(DoFunc3, nullptr);
    return 0;
}