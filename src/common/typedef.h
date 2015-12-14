////////////////////////////////////////////////////////
/// \file typedef.h
/// \brief define fixed length type
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date June 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr
////////////////////////////////////////////////////////

#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <cstdint>
#include <sstream>
#include <cassert>
#include <limits>

// Signed int
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

// Unsigned int
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

// Float
typedef float float32;
typedef double float64;
typedef float32 float_t;

/// \brief Convert integer test to boolean
template <typename InType>
inline bool Succeeded(const InType& test)
{
	return ((int64)test >= 0);
}

/// \brief Convert integer test to boolean
template <typename InType>
inline bool Failed(const InType& test)
{
	return !Succeeded(test);
}

/// \brief Convert boolean to string
inline const char* Bool2Str(bool val)
{
	return val ? "True" : "False";
}

/// \brief Convert boolean to 'check' message
inline const char* Bool2Check(bool val)
{
	return val ? "Ok" : "Failed!";
}

// Type conversion
template <typename Otype, typename Itype>
inline Otype Convert(const Itype& ival)
{
	std::stringstream stream;
	stream << ival;
	Otype oval;
	stream >> oval;
	return oval;
}

#ifdef LOWEST_NOT_DEFINED
template<typename T,
	bool B = std::is_unsigned<T>::value>
struct Limit
{
	static T greatest()
	{
		return (std::numeric_limits<T>::max)();
	}
	static T lowest()
	{
		return -(std::numeric_limits<T>::max)();
	}
	static T min_value()
	{
		return (std::numeric_limits<T>::min)();
	}
	static T max_value()
	{
		return greatest();
	}
};

template<typename T>
struct Limit<T, true>
{
	static T greatest()
	{
		return (std::numeric_limits<T>::max)();
	}
	static T lowest()
	{
		return T(0);
	}
	static T min_value()
	{
		return (std::numeric_limits<T>::min)();
	}
	static T max_value()
	{
		return greatest();
	}
};
#else

template<typename T,
	bool B = std::is_integral<T>::value>
struct Limit
{
	static T greatest()
	{
		return (std::numeric_limits<T>::max)();
	}
	static T lowest()
	{
		return (std::numeric_limits<T>::lowest)();
	}
	static T min_value()
	{
		return (std::numeric_limits<T>::min)();
	}
	static T max_value()
	{
		return greatest();
	}
	static T epsilon()
	{
		return std::numeric_limits<T>::epsilon();
	}
	// Lowest value that can be subtracted from max_value
	static T max_epsilon()
	{
		return epsilon() * max_value();
	}
};

template<typename T>
struct Limit<T,true>
{
	static T greatest()
	{
		return (std::numeric_limits<T>::max)();
}
	static T lowest()
	{
		return (std::numeric_limits<T>::lowest)();
	}
	static T min_value()
	{
		return (std::numeric_limits<T>::min)();
	}
	static T max_value()
	{
		return greatest();
	}
	static T epsilon()
	{
		return T(1);
	}
	static T max_epsilon()
	{
		return T(1);
	}
};
#endif


#ifdef CONSTEXPR_EXIST
#define CONSTEXPR constexpr
#else
#define CONSTEXPR
#endif


#endif
