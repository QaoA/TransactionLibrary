#include <iostream>
#include "SLTList.h"
#include "CLThreadTransactionManager.h"


using namespace std;
using namespace NVMTransaction;

int main()
{
	//SLTList * pList = SLTList::MakeList(0, 5);
	//SLTList::ShowList(pList);
	//cout << "pList addr : 0x" << hex << (unsigned long)(pList) << endl;

	NVMMalloc::Recovery();
	CLThreadTransactionManager::GetInstance().RunWriteTransaction(SLTList::ShowListReadOnly, (void *)0x7fffb6fd8fe0);
	//CLThreadTransactionManager::GetInstance().RunWriteTransaction(SLTList::IncreaseAll, (void *)0x7fffb6fd8fe0);
	//SLTList::ShowListReadOnly((void *)0x7fffb6fd8fe0);
	
	//gc
	//new/delete
	//CLTransactionalObject::WriteCommit
    return 0;
}