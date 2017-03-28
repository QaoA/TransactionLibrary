#ifndef __BASIC_FUNCTIONS_H__
#define __BASIC_FUNCTIONS_H__

template<typename T>
inline const T Min(const T & first, const T & second)
{
	return first < second ? first : second;
}

template<typename T>
inline const T Max(const T & first, const T & second)
{
	return first < second ? second : first;
}

#endif