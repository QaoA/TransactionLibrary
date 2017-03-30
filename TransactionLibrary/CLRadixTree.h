#ifndef __RADIX_TREE_H__
#define __RADIX_TREE_H__

#include "CLRadixTreeNode.h"
#include "CLMutex.h"
#include "CLCriticalSection.h"
#include <cassert>

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
	LeafType * GetOrCreate(void * key, CreateFunc leafCreateFunc, void * createArgs);

private:
	CLRadixTreeNode m_root;
	CLMutex m_lock;
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
	CLCriticalSection cs(&m_lock);
	m_root.Insert(reinterpret_cast<unsigned long>(key), pNewNode, SLOT_INIT_MASK);
}

template<class LeafType>
LeafType * CLRadixTree<LeafType>::Get(void * key)
{
	CLCriticalSection cs(&m_lock);
	return static_cast<LeafType *>(m_root.Get(reinterpret_cast<unsigned long>(key), SLOT_INIT_MASK));
}

template<class LeafType>
LeafType * CLRadixTree<LeafType>::GetAndRemove(void * key)
{
	CLCriticalSection cs(&m_lock);
	return static_cast<LeafType *>(m_root.Remove(reinterpret_cast<unsigned long>(key), SLOT_INIT_MASK));
}

template<class LeafType>
inline LeafType * CLRadixTree<LeafType>::GetOrCreate(void * key, CreateFunc leafCreateFunc, void * createArgs)
{
	CLCriticalSection cs(&m_lock);
	LeafType * pReturnLeaf = static_cast<LeafType *>(m_root.Get(reinterpret_cast<unsigned long>(key), SLOT_INIT_MASK));
	if (pReturnLeaf == nullptr)
	{
		pReturnLeaf = CreateFunc(key);
		if (pReturnLeaf != nullptr)
		{
			m_root.Insert(reinterpret_cast<unsigned long>(key), pNewNode, SLOT_INIT_MASK);
		}
	}
	return pReturnLeaf;
}

#endif