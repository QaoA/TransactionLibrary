#include "CLNameAddressPairSet.h"
#include "CLNameAddressPair.h"
#include <cstring>
#include <cassert>

TRANSACTIONLIB_NS_BEGIN

CLNameAddressPairSet::CLNameAddressPairSet()
{
}

CLNameAddressPairSet::~CLNameAddressPairSet()
{
}

CLNameAddressPair * CLNameAddressPairSet::FindByName(char * strName)
{
	for (CLNameAddressPair * pPair : m_set)
	{
		if (strcmp(strName, pPair->GetName()) == 0)
		{
			return pPair;
		}
	}
	return nullptr;
}

void CLNameAddressPairSet::AddObject(CLNameAddressPair * pPair)
{
	assert(pPair);
	m_set.push_back(pPair);
}

void CLNameAddressPairSet::Clear()
{
	m_set.erase(m_set.begin(), m_set.end());
}

void CLNameAddressPairSet::Commit(CLLogItemsSet & logItemSet)
{
	for (CLNameAddressPair * pPair : m_set)
	{
		pPair->Commit(logItemSet);
	}
}

void CLNameAddressPairSet::Abort()
{
	for (CLNameAddressPair * pPair : m_set)
	{
		pPair->Abort();
	}
}

void CLNameAddressPairSet::Close()
{
	for (CLNameAddressPair * pPair : m_set)
	{
		pPair->Close();
	}
}

TRANSACTIONLIB_NS_END