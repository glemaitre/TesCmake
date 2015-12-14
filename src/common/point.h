/// \file point.h
/// \brief Point 2D and 3D
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date June 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef CORE_POINT_H
#define CORE_POINT_H

#include "smallvector.h"
#include "nodata.h"
#include "typedef.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <limits>

namespace core
{

template<typename T,int32 I>
struct Point : public SmallVector<T, I>
{
	typedef SmallVector<T, I> vector_t;
	// Default constructor
	Point()
	{}
	// Constructor with vector
	Point(const vector_t& v)
		:vector_t(v)
	{}
	// Explicit constructor with value
	explicit Point(const T& val)
		:vector_t(val)
	{}

	bool valid()const
	{
		return CodeValue<T>::is_valid((*this)[0]);
	}
	bool unmasked()const
	{
		return CodeValue<T>::is_valid((*this)[0]);
	}
	bool reliable()const
	{
		return CodeValue<T>::is_reliable((*this)[0]);
	}
	bool finite()const
	{
		return CodeValue<T>::is_finite((*this)[0]);
	}
	bool is_data()const
	{
		return CodeValue<T>::is_data((*this)[0]);
	}
	bool masked()const
	{
		return !valid();
	}

	bool unreliable()const
	{
		return !reliable();
	}

	bool infinite()const
	{
		return !finite();
	}

	bool no_data()const
	{
		return !is_data();
	}

	void set_masked()
	{
		(*this)[0] = CodeValue<T>::masked();
	}

	void set_unreliable()
	{
		(*this)[0] = CodeValue<T>::unreliable();
	}

	void set_infinite()
	{
		(*this)[0] = CodeValue<T>::infinite();
	}

	void set_no_data()
	{
		(*this)[0] = CodeValue<T>::no_data();
	}
	void invalidate()
	{
		(*this)[0] = CodeValue<T>::no_data();
	}
};

template<typename T,int32 I>
inline T QuadDist(const Point<T, I>& pt1, const Point<T, I>& pt2)
{
	return QuadNorm(pt2 - pt1);
}

template<typename T, int32 I>
inline float_t Dist(const Point<T, I>& pt1, const Point<T, I>& pt2)
{
	return sqrt(float_t(QuadDist(pt1, pt2)));
}

/// \brief point 2D
template<typename T = float_t>
struct Point2D : public Point<T,2>
{
    typedef Point<T,2> point_t;
    typedef typename point_t::vector_t vector_t;
	/// \brief Default constructor
	Point2D(){}
	/// \brief Constructor
	Point2D(const T& x, const T& y):
		point_t({ x, y })
	{}
    /// \brief Constructor
    Point2D(const T* st):
		point_t(st)
    {}
    /// \brief Constructor
    Point2D(std::initializer_list<T> il):
		point_t(il)
    {}
	/// \brief Constructor with a vector
	Point2D(const vector_t& vec) :
		point_t(vec)
	{}
    // Access to x
    const T& x()const
    {
        return (*this)[0];
    }
    T& x()
    {
        return (*this)[0];
    }
    // Access to y
    const T& y()const
    {
        return (*this)[1];
    }
    T& y()
    {
        return (*this)[1];
    }
};

/// \brief point 3D
template<typename T = float_t>
struct Point3D : public Point<T,3>
{
	typedef Point<T, 3> point_t;
	typedef typename point_t::vector_t vector_t;
	/// \brief Default constructor
	Point3D(){}
	/// \brief Constructor
	Point3D(const T& x, const T& y,const T& z = 0):
		point_t({ x, y, z })
	{}
    /// \brief Constructor
    Point3D(const T* st):
		point_t(st)
    {}
    /// \brief Constructor
    Point3D(std::initializer_list<T> il):
		point_t(il)
    {}
	/// \brief Constructor with a vector
	Point3D(const vector_t& vec) :
		point_t(vec)
	{}
    // Access to x
    const T& x()const
    {
        return (*this)[0];
    }
    T& x()
    {
        return (*this)[0];
    }
    // Access to y
    const T& y()const
    {
        return (*this)[1];
    }
    T& y()
    {
        return (*this)[1];
    }
    // Access to z
    const T& z()const
    {
        return (*this)[2];
    }

    T& z()
    {
        return (*this)[2];
    }
};

/// \brief Put the point in a string
template<typename T>
inline std::string ToString(const Point2D<T>& pt)
{
	std::stringstream msg;
	msg << pt.x() << ", " << pt.y() ;
	return msg.str();
}

/// \brief Put the point in ostream
template<typename T>
inline std::ostream& operator<<(std::ostream& os,
	const Point2D<T>& pt)
{
	return os << ToString(pt);
}

/// \brief Put the point in a string
template<typename T>
inline std::string ToString(const Point3D<T>& pt)
{
	std::stringstream msg;
	msg << pt.x() << ", " << pt.y() << ", " << pt.z();
	return msg.str();
}

/// \brief Put the point in ostream
template<typename T>
inline std::ostream& operator<<(std::ostream& os,
	const Point3D<T>& pt)
{
	return os << ToString(pt);
}

template<class Pt>
using Points = std::vector<Pt>;

} // core
#endif
