////////////////////////////////////////////////////////
/// \file pushpop.h
/// \brief Functions to push and pop data in/out a buffer
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date June 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr
////////////////////////////////////////////////////////

#ifndef UTIL_PUSHPOP_H
#define UTIL_PUSHPOP_H

#include "typedef.h"
#include <string>
#include "string.h"

namespace util
{
#ifndef ARM

/// \brief Read value in a buffer
template<typename T>
inline T GetValue(const char* buffer, uint32 offset = 0)
{
	return *(reinterpret_cast<const T*>(buffer + offset));
}
template<typename T>
inline T GetValue(char* buffer, uint32 offset = 0)
{
	return *(reinterpret_cast<const T*>(buffer + offset));
}
/// \brief Write value in a buffer
template<typename T>
inline void SetValue(const T& val, char* buffer, uint32 offset = 0)
{
	*reinterpret_cast<T*> (buffer+offset) = val;;
}


#else

/// \brief Read value in a buffer
template<typename T>
inline T GetValue(const char* buffer, uint32 offset = 0)
{
	T t;
	memcpy(reinterpret_cast<void*>(&t), buffer + offset, sizeof(T));
	return t;
}
template<typename T>
inline T GetValue(char* buffer, uint32 offset = 0)
{
	T t;
	memcpy(reinterpret_cast<void*>(&t), buffer + offset, sizeof(T));
	return t;
}
/// \brief Write value in a buffer
template<typename T>
inline void SetValue(const T& val, char* buffer, uint32 offset = 0)
{
	memcpy(buffer+offset, reinterpret_cast<const void*>(&val), sizeof(T));
}
#endif
/// \brief Read value in a buffer
/// and move the pointer to the next data
template<typename T>
inline T PopValue(const char*& buffer)
{
	T t = GetValue<T>(buffer);
	buffer += sizeof(T);
	return t;
}
template<typename T>
inline T PopValue(char*& buffer)
{
	T t = GetValue<T>(buffer);
	buffer += sizeof(T);
	return t;
}

template<typename T>
inline const char* PopValue(T& t, const char*& buffer)
{
	t = GetValue<T>(buffer);
	buffer += int32(sizeof(T));
	return buffer;
}

template<typename T>
inline char* PopValue(T& t, char*& buffer)
{
	t = GetValue<T>(buffer);
	buffer += int32(sizeof(T));
	return buffer;
}

template<>
inline const char* PopValue(std::string& str, const char*& buffer)
{
	uint8 siz = *buffer;
	str.assign(buffer + 1, siz);
	buffer += siz + 1;
	return buffer;
}

template<>
inline char* PopValue(std::string& str, char*& buffer)
{
	uint8 siz = *buffer;
	str.assign(buffer + 1, siz);
	buffer += siz + 1;
	return buffer;
}

template<typename T>
inline const char* PopValue(T*& pt, int32 siz, const char*& buffer)
{
	memcpy(pt, buffer, siz*sizeof(T));
	buffer += siz + 1;
	return buffer;
}

template<typename T>
inline char* PopValue(T*& pt, int32 siz, char*& buffer)
{
	memcpy(pt, buffer, siz*sizeof(T));
	buffer += siz + 1;
	return buffer;
}

/// \brief Push data in a buffer
template<typename T>
inline char* PushValue(const T& val, char*& buffer)
{
	SetValue(val, buffer);
	buffer += int32(sizeof(T));
	return buffer;
}

template<>
inline char* PushValue(const std::string& str, char*& buffer)
{
	assert(str.size() < 256);
	uint8 siz = str.size();
	*buffer = siz;
	memcpy(buffer + 1, str.c_str(), siz);
	buffer += siz + 1;
	return buffer;
}
} // util

#endif
