#include <iostream>
#include "ListTestFuncs.h"
#include "TestFuncs.h"
#include "TCLTimeCount.h"
#include <pthread.h>
#include "CLMutex.h"

using namespace std;
using namespace NVMTransaction;

int main()
{
	
	TCLTimeCount timeCounter;
	timeCounter.TimeCountStart();

	timeCounter.TimeCountEnd();
	cout << timeCounter.GetDelta() << endl;
    return 0;
}