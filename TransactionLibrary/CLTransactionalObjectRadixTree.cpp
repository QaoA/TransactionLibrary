#include "CLTransactionalObjectRadixTree.h"
#include "CLTransactionalObject.h"
#include <cassert>

TRANSACTIONLIB_NS_BEGIN

#define GetTimeStamp(version) (version >> 1)
#define MakeTimeStamp(version) ((version << 1) | 0x1)
#define IsTimeStamp(version) (version & 0x1)

CLTransactionalObjectRadixTree::CLTransactionalObjectRadixTree()
{
}

CLTransactionalObjectRadixTree::~CLTransactionalObjectRadixTree()
{
}

CLTransactionalObject * CLTransactionalObjectRadixTree::FindOrCreate(void * pNVMObject, SLTransactionalObjectCreatArgs * transactionalObjectCreateArgs)
{
	unsigned long key = (unsigned long)pNVMObject;
	unsigned long mask = RTREE_INIT_MASK;
	SLRadixTreeNode * pNode = &m_root;
	for (int i = RTREE_TREE_HEIGHT; i > 1; --i)
	{
		unsigned int index = (key & mask) >> (RTREE_SUBNODE_NUM_BIT * (i - 1));
		if (pNode->m_slots[index].m_subNode == nullptr)
		{
			pNode->m_slots[index].m_subNode = new SLRadixTreeNode();
		}
		pNode = pNode->m_slots[index].m_subNode;
		mask >>= RTREE_SUBNODE_NUM_BIT;
	}
	unsigned int index = key & mask;
	if (IsTimeStamp(pNode->m_slots[index].m_lastCommitTimeStamp) || pNode->m_slots[index].m_object == nullptr)
	{
		pNode->m_slots[index].m_object = CLTransactionalObject::MakeObject(transactionalObjectCreateArgs, GetTimeStamp(pNode->m_slots[index].m_lastCommitTimeStamp));
	}
	return pNode->m_slots[index].m_object;
}

CLTransactionalObject * CLTransactionalObjectRadixTree::GetAndRemove(void * pNVMObject)
{
	unsigned long key = (unsigned long)pNVMObject;
	unsigned long mask = RTREE_INIT_MASK;
	SLRadixTreeNode * pNode = &m_root;
	for (int i = RTREE_TREE_HEIGHT; i > 1; --i)
	{
		unsigned int index = (key & mask) >> (RTREE_SUBNODE_NUM_BIT * (i - 1));
		pNode = pNode->m_slots[index].m_subNode;
		assert(pNode);
		mask >>= RTREE_SUBNODE_NUM_BIT;
	}

	unsigned int index = key & mask;
	assert(pNode && !IsTimeStamp(pNode->m_slots[index].m_lastCommitTimeStamp));
	CLTransactionalObject * pObject = pNode->m_slots[index].m_object;
	pNode->m_slots[index].m_lastCommitTimeStamp = MakeTimeStamp(pNode->m_slots[index].m_object->GetLastCommitTimeStamp());
	return pObject;
}

CLTransactionalObject * CLTransactionalObjectRadixTree::GetAndDelete(void * pNVMObject)
{
	unsigned long key = (unsigned long)pNVMObject;
	unsigned long mask = RTREE_INIT_MASK;
	SLRadixTreeNode * pNode = &m_root;
	for (int i = RTREE_TREE_HEIGHT; i > 1; --i)
	{
		unsigned int index = (key & mask) >> (RTREE_SUBNODE_NUM_BIT * (i - 1));
		pNode = pNode->m_slots[index].m_subNode;
		assert(pNode);
		mask >>= RTREE_SUBNODE_NUM_BIT;
	}

	unsigned int index = key & mask;
	assert(pNode && !IsTimeStamp(pNode->m_slots[index].m_lastCommitTimeStamp));
	CLTransactionalObject * pObject = pNode->m_slots[index].m_object;
	pNode->m_slots[index].m_lastCommitTimeStamp = LSA_TIME_STAMP_START;
	return pObject;
}

TRANSACTIONLIB_NS_END