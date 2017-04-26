#ifndef __TRANSACTIONAL_OBJECT_RADIX_TREE_H__
#define __TRANSACTIONAL_OBJECT_RADIX_TREE_H__

#include "LSATimeStamp.h"
#include "TransactionLibraryNameSpace.h"

TRANSACTIONLIB_NS_BEGIN

#define RTREE_SUBNODE_NUM_BIT 6
#define RTREE_SUBNODE_NUM (1 << RTREE_SUBNODE_NUM_BIT)
#define RTREE_TREE_HEIGHT 8
#define RTREE_INIT_MASK (0xfc0000000000)

class CLTransactionalObject;
struct SLTransactionalObjectCreatArgs;

class CLTransactionalObjectRadixTree
{
private:
	struct SLRadixTreeNode;
	struct SLRadixTreeNodeSlot
	{
		union
		{
			SLRadixTreeNode * m_subNode;
			CLTransactionalObject * m_object;
			LSATimeStamp m_lastCommitTimeStamp;
		};
	};
	struct SLRadixTreeNode
	{
		SLRadixTreeNodeSlot m_slots[RTREE_SUBNODE_NUM];
	};

public:
	CLTransactionalObjectRadixTree();
	~CLTransactionalObjectRadixTree();

public:
	CLTransactionalObject * FindOrCreate(void * pNVMObject, SLTransactionalObjectCreatArgs * transactionalObjectCreateArgs);
	CLTransactionalObject * GetAndRemove(void * pNVMObject);
	CLTransactionalObject * GetAndDelete(void * pNVMObject);

private:
	SLRadixTreeNode m_root;
};

TRANSACTIONLIB_NS_END

#endif