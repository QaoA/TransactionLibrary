#include <iostream>
#include "SLTList.h"
#include "CLThreadTransactionManager.h"


using namespace std;
using namespace NVMTransaction;

int main()
{
	CLThreadTransactionManager::GetInstance().RunWriteTransaction(SLTList::MakeList, (void *)0x7fffb6fd8fe0);
	CLThreadTransactionManager::GetInstance().RunReadTransaction(SLTList::ShowListReadOnly, (void *)0x7fffb6fd8fe0);

//	NVMMalloc::Recovery();
//	CLThreadTransactionManager::GetInstance().RunReadTransaction(SLTList::ShowListReadOnly, (void *)0x7fffb6fd8fe0);
//	CLThreadTransactionManager::GetInstance().RunWriteTransaction(SLTList::IncreaseAll, (void *)0x7fffb6fd8fe0);
//	CLThreadTransactionManager::GetInstance().RunReadTransaction(SLTList::ShowListReadOnly, (void *)0x7fffb6fd8fe0);
	//SLTList::ShowListReadOnly((void *)0x7fffb6fd8fe0);
	
	//gc
	//new/delete
	//CLTransactionalObject::WriteCommit

    return 0;
}