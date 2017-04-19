#include "CLRadixTreeNode.h"
#include "CLCriticalSection.h"
#include <cstring>
#include <cassert>

TRANSACTIONLIB_NS_BEGIN

CLRadixTreeNode::CLRadixTreeNode(const unsigned int height) :
m_height(height),
m_count(0)
{
	memset(m_slots, 0, sizeof(m_slots));
}

CLRadixTreeNode::~CLRadixTreeNode()
{
}

void CLRadixTreeNode::Insert(unsigned long key, void * pLeaf, unsigned long mask)
{
	unsigned long index = ((key & mask) >> (m_height*SLOT_COUNT_BIT));
	if (m_height != 0)
	{
		if (m_slots[index].m_pNextLayerNode == nullptr)
		{
			m_slots[index].m_pNextLayerNode = new CLRadixTreeNode(m_height - 1);
			++m_count;
		}
		m_slots[index].m_pNextLayerNode->Insert(key, pLeaf, mask >> SLOT_COUNT_BIT);
	}
	else
	{
		assert(m_slots[index].m_leaf == nullptr);
		m_slots[index].m_leaf = pLeaf;
		++m_count;
	}
}

void * CLRadixTreeNode::Remove(unsigned long key, unsigned long mask)
{
	unsigned long index = ((key & mask) >> (m_height*SLOT_COUNT_BIT));
	if (m_height != 0)
	{
		assert(m_slots[index].m_pNextLayerNode != nullptr);
		void * pLeaf = m_slots[index].m_pNextLayerNode->Remove(key, mask >> SLOT_COUNT_BIT);
		if (m_slots[index].m_pNextLayerNode->m_count == 0)
		{
			delete m_slots[index].m_pNextLayerNode;
			m_slots[index].m_pNextLayerNode = nullptr;
			--m_count;
		}
		return pLeaf;
	}
	else
	{
		void * pLeaf = m_slots[index].m_leaf;
		m_slots[index].m_leaf = nullptr;
		--m_count;
		return pLeaf;
	}
}

void * CLRadixTreeNode::Get(unsigned long key, unsigned long mask)
{
	unsigned long index = ((key & mask) >> (m_height*SLOT_COUNT_BIT));
	if (m_height != 0)
	{
		void * pLeaf = nullptr;
		if (m_slots[index].m_pNextLayerNode != nullptr)
		{
			pLeaf = m_slots[index].m_pNextLayerNode->Get(key, mask >> SLOT_COUNT_BIT);
		}
		else
		{
			return nullptr;
		}
		return pLeaf;
	}
	else
	{
		return m_slots[index].m_leaf;
	}
}

TRANSACTIONLIB_NS_END