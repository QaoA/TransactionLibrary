#include <iostream>
#include "NVMMalloc.h"

using namespace std;

int main()
{
	//SLTList * pList = SLTList::MakeList(0, 5);
	//cout << "helloworld" << endl;
	void * addr[3];
	for (int i = 0; i < 3; ++i)
	{
		addr[i] = NVMMalloc::MallocOnNVM(8);
	}
    return 0;
}