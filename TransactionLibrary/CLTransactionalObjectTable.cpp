#include "CLTransactionalObjectTable.h"

CLTransanctionalObjectTable::CLTransanctionalObjectTable()
{
}

CLTransanctionalObjectTable::~CLTransanctionalObjectTable()
{
}

CLTransanctionalObjectTable & CLTransanctionalObjectTable::GetInstance()
{
	static CLTransanctionalObjectTable instance;
	return instance;
}
