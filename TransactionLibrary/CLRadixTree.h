#ifndef __RADIX_TREE_H__
#define __RADIX_TREE_H__

#include "CLRadixTreeNode.h"
#include "TransactionLibraryNameSpace.h"
#include <cassert>

TRANSACTIONLIB_NS_BEGIN

static const unsigned int TREE_HEIGHT = 7;

template<class LeafType>
class CLRadixTree
{
public:
	typedef LeafType*(*CreateFunc)(void *);

public:
	CLRadixTree();
	~CLRadixTree();
	
public:
	void Insert(LeafType * pNewNode, void * key);
	LeafType * Get(void * key);
	LeafType * GetAndRemove(void * key);

private:
	CLRadixTreeNode m_root;
};

template<class LeafType>
CLRadixTree<LeafType>::CLRadixTree():
m_root(TREE_HEIGHT)
{
}

template<class LeafType>
CLRadixTree<LeafType>::~CLRadixTree()
{
}

template<class LeafType>
void CLRadixTree<LeafType>::Insert(LeafType * pNewNode, void * key)
{
	assert(pNewNode);
	m_root.Insert(reinterpret_cast<unsigned long>(key), pNewNode, SLOT_INIT_MASK);
}

template<class LeafType>
LeafType * CLRadixTree<LeafType>::Get(void * key)
{
	return static_cast<LeafType *>(m_root.Get(reinterpret_cast<unsigned long>(key), SLOT_INIT_MASK));
}

template<class LeafType>
LeafType * CLRadixTree<LeafType>::GetAndRemove(void * key)
{
	return static_cast<LeafType *>(m_root.Remove(reinterpret_cast<unsigned long>(key), SLOT_INIT_MASK));
}

TRANSACTIONLIB_NS_END
#endif