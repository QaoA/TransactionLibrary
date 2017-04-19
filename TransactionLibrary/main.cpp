#include <iostream>
#include "SLTList.h"

using namespace std;

int main()
{
	//SLTList * pList = SLTList::MakeList(0, 5);
	//SLTList::ShowList(pList);
	//cout << "pList addr : 0x" << hex << (unsigned long)(pList) << endl;

	NVMMalloc::Recovery();
	SLTList::ShowListReadOnly((SLTList *)0x7fffb6fd8fe0);
	
    return 0;
}