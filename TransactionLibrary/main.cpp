#include <iostream>
#include "CLTransactionAPIs.h"
#include "TSLList.h"
#include "NVMMalloc.h"

using namespace std;
using namespace NVMTransaction;

void DoFunc(void *)
{
	TSLList * pHead = TSLList::MakeList();
	CLTransactionAPIs::SetAddressByName("listHead", pHead);
	TSLList::Show(pHead);
}

void DoFunc2(void *)
{
	TSLList * pHead = (TSLList *)CLTransactionAPIs::GetAddressByName("listHead");
	TSLList::Show(pHead);
}

void DoFunc3(void *)
{
	NVMMalloc::Recovery();
	TSLList * pHead = (TSLList *)CLTransactionAPIs::GetAddressByName("listHead");
	TSLList::Show(pHead);
}

int main()
{
	//CLTransactionAPIs::RunWriteTransaction(DoFunc, nullptr);
	CLTransactionAPIs::RunReadTransaction(DoFunc3, nullptr);
    return 0;
}