#ifndef __TRANSACTIONAL_OBJECT_FLAG_H__
#define __TRANSACTIONAL_OBJECT_FLAG_H__

struct SLTransactionalObjectFlag
{
public:
	inline void SetWrite();
	inline void ClearWrite();
	inline bool IsWriting();
	inline void SetNew();
	inline void ClearNew();
	inline bool IsNew();
	inline void SetDelete();
	inline void ClearDelete();
	inline bool IsDelete();

private:
	bool m_write : 3;
	bool m_new : 3;
	bool m_delete : 2;
};

inline void SLTransactionalObjectFlag::SetWrite()
{
	m_write = true;
}

inline void SLTransactionalObjectFlag::ClearWrite()
{
	m_write = false;
}

inline bool SLTransactionalObjectFlag::IsWriting()
{
	return m_write;
}

inline void SLTransactionalObjectFlag::SetNew()
{
	m_new = true;
}

inline void SLTransactionalObjectFlag::ClearNew()
{
	m_new = false;
}

inline bool SLTransactionalObjectFlag::IsNew()
{
	return m_new;
}

inline void SLTransactionalObjectFlag::SetDelete()
{
	m_delete = true;
}

inline void SLTransactionalObjectFlag::ClearDelete()
{
	m_delete = false;
}

inline bool SLTransactionalObjectFlag::IsDelete()
{
	return m_delete;
}

#endif