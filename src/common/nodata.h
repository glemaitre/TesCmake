/// \file nodata.h
/// \brief No data codes
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date June 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef CORE_NO_DATA_H
#define CORE_NO_DATA_H

#include "typedef.h"

namespace core
{
template<class T>
struct CodeValueT
{
	enum
	{
		no_data_code,
		infinite_code,
		unreliable_code,
		masked_code,
	};
	// No data representation
	static T no_data()
	{
		return Limit<T>::max_value()
			- no_data_code * Limit<T>::max_epsilon();
	}
	// Invalid representation
	static T infinite()
	{
		return Limit<T>::max_value()
			- infinite_code * Limit<T>::max_epsilon();
	}
	// Unreliable representation
	static T unreliable()
	{
		return Limit<T>::max_value()
			- unreliable_code * Limit<T>::max_epsilon();
	}
	// Masked representation
	static T masked()
	{
		return Limit<T>::max_value()
			- masked_code * Limit<T>::max_epsilon();
	}
};

template<typename T>
struct CodeValue
{
public:
	typedef T value_type;
	enum
	{
		no_data_code,
		undefined_code,
		infinite_code,
		unreliable_code,
		masked_code,
	};
private:
	static const value_type no_data_;
	static const value_type undef_;
	static const value_type infinite_;
	static const value_type unreliable_;
	static const value_type masked_;
public:

	// No data representation
	static value_type no_data()
	{
		return no_data_;
	}
	// Undefined representation
	static value_type undefined()
	{
		return undef_;
	}
	// Invalid representation
	static value_type infinite()
	{
		return infinite_;
	}
	// Unreliable representation
	static value_type unreliable()
	{
		return unreliable_;
	}
	// Masked representation
	static value_type masked()
	{
		return masked_;
	}

	static bool is_data(const T& val)
	{
		return val < no_data();
	}
	static bool is_defined(const T& val)
	{
		return val < undefined();
	}
	static bool is_finite(const T& val)
	{
		return val < infinite();
	}
	static bool is_reliable(const T& val)
	{
		return val < unreliable();
	}
	static bool is_valid(const T& val)
	{
		return val < masked();
	}
};
} // core
#endif
