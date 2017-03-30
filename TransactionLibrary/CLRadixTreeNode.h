#ifndef __RADIX_TREE_NODE_H__
#define __RADIX_TREE_NODE_H__

#include <vector>

#define SLOT_COUNT_BIT 6
#define SLOT_COUNT (1<<SLOT_COUNT_BIT)
#define SLOT_INIT_MASK (0xfc0000000000)

class CLRadixTreeNode
{
public:
	union SLRadixTreeSlot
	{
		CLRadixTreeNode * m_pNextLayerNode;
		void * m_leaf;
	};

public:
	CLRadixTreeNode(const unsigned int height);
	~CLRadixTreeNode();

public:
	void Insert(unsigned long key, void * pLeaf, unsigned long mask);
	void * Remove(unsigned long key, unsigned long mask);
	void * Get(unsigned long key, unsigned long mask);

private:
	const unsigned int m_height;
	unsigned int m_count;
	SLRadixTreeSlot m_slots[SLOT_COUNT];
};

#endif