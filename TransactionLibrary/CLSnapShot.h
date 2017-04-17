#ifndef __SNAPSHOT_H__
#define __SNAPSHOT_H__

#include "LSATimeStamp.h"
#include "CLBasicData.h"
#include "CLLSAClock.h"
#include "BasicFunctions.h"

class CLSnapShot
{
public:
	CLSnapShot();
	~CLSnapShot();

public:
	void Reset();

public:
	inline bool HasIntersection(LSATimeStamp lower);
	inline bool IsValid();
	inline LSATimeStamp GetUpper();
	inline void TrySetUpper(LSATimeStamp time);
	inline void TrySetLower(LSATimeStamp time);
	inline void ExtendUpper(LSATimeStamp time);

private:
	LSATimeStamp m_lower;
	LSATimeStamp m_upper;
};

inline bool CLSnapShot::HasIntersection(LSATimeStamp lower)
{
	return lower <= m_upper;
}

inline bool CLSnapShot::IsValid()
{
	return m_lower <= m_upper;
}

inline LSATimeStamp CLSnapShot::GetUpper()
{
	return m_upper;
}

inline void CLSnapShot::TrySetUpper(LSATimeStamp time)
{
	if (time == LSA_TIME_STAMP_INFINITE)
	{
		time = CLBasicData::GetInstance().GetLSAClock().GetClock();
	}
	if (m_upper > time)
	{
		m_upper = time;
	}
}

inline void CLSnapShot::TrySetLower(LSATimeStamp time)
{
	if (m_lower < time)
	{
		m_lower = time;
	}
}

inline void CLSnapShot::ExtendUpper(LSATimeStamp readedObjectsValidUpper)
{
	m_upper = Min(CLBasicData::GetInstance().GetLSAClock().GetClock(), readedObjectsValidUpper);
}

#endif