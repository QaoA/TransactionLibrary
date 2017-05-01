#ifndef __NORMAL_LIST_H__
#define __NORMAL_LIST_H__

struct TSLNormalList
{
public:
	TSLNormalList * m_pPrevious;
	TSLNormalList * m_pNext;
	int m_data;

public:
	static void Init(TSLNormalList * pList, int data);
	static void Append(TSLNormalList * pList, TSLNormalList * pPrevious);
	static void Show(TSLNormalList * pList);
	static TSLNormalList * MakeList();
};

#endif