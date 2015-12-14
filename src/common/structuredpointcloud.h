/// \file structuredpointcloud.h
/// \brief Structured point cloud class
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date September 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef CORE_STRUCTURED_POINTCLOUD_H
#define CORE_STRUCTURED_POINTCLOUD_H

#include "point.h"
#include "box.h"
#include "typedef.h"
#include "matrix.h"

namespace core
{
// Structured point cloud
template<class Pt = Point3D<float_t> >
class StructuredPointCloud :
	public Matrix< Pt >
{
protected:
    typedef Matrix< Pt > Mat;
	int64 timestamp_ = undef_time;
public:
    typedef Matrix<Pt> container_t;
	typedef typename container_t::Storage Storage;
	typedef typename Pt::value_type value_type;
	typedef Pt point_type;

	typedef point_type* iterator;
	typedef const point_type* const_iterator;

	enum
    {
        ndim = Pt::nelem,
        undef_time = 0,
    };
	/// \brief Default constructor
	StructuredPointCloud()
	{}
	/// \brief Constructor
	StructuredPointCloud(uint32 w, uint32 h,
		Pt* buffer,
		uint32 step_x = 0):
		container_t(h,w,nullptr,buffer)
	{}
	/// \brief Begin
	iterator begin()
	{
		return this->data();
	}
	const_iterator begin()const
	{
		return this->data();
	}

	iterator end()
	{
		return this->data() + Mat::size_x() * Mat::size_y();
	}
	const_iterator end()const
	{
		return this->data() + Mat::size_x() * Mat::size_y();
	}
	/// \brief Initialization
	void init(uint32 w, uint32 h)
	{
		// Warning inversion: w==y, h==x!
		container_t::init(h, w);
	}
	/// \brief Initialization with points
	void init(uint32 w, uint32 h,
		int64 time,
		const Pt* pts,uint32 stride = 0)
	{
		// Warning inversion: w==y, h==x!
		container_t::init(h, w, nullptr,pts,stride);
		timestamp_ = time;
	}
	/// \brief Initialization with points
	template<class Pts>
	void init(uint32 w, uint32 h,const Pts& pts)
	{
		// Warning inversion: w==y, h==x!
		container_t::init(h, w,pts.storage(),
			pts.data(),0);
		timestamp_ = pts.timestamp();
	}
	/// \brief Allocate memory with current w and h
	void create()
	{
		container_t::create();
	}
	/// \brief Allocate memory
	void create(uint32 w, uint32 h)
	{
		container_t::create(h,w);
	}

	/// \brief Initialization with depth
	template<typename T>
	void create(uint32 w,uint32 h,
		T* depth,
		value_type scale = 1,
		uint32 stride = 0);

	/// \brief Create from unstructured point cloud
	template<class Pts>
	void create(uint32 w, uint32 h, const Pts& pts);
	/// \brief Fill with unsstructured point cloud
	template<class Pts>
	void fill(const Pts& pts);
	/// \brief Get width
	uint32 width()const
	{
		// Warning inversion: w==y!
		return container_t::size_y();
	}

	/// \brief Get height
	uint32 height()const
	{
		// Warning inversion: h==x!
		return container_t::size_x();
	}
	/// \brief Get timestamp
	int64 timestamp()const
	{
		return timestamp_;
	}
	/// \brief Set timestamp
	void timestamp(int64 val)
	{
		timestamp_ = val;
	}
	/// \brief Emtpy?
	bool empty()const;

    /// \brief Number of valid data
    uint32 size()const;

    /// \brief Update a bounding box
	bool bounding_box(Box<value_type, ndim>& box)const;
	/// \brief Shortest distance
	value_type distance(const Pt& pt)const;
	/// \brief Shortest distance along axis
	value_type distance(const Pt& origin, const Pt& direction,
		value_type epsilon = 1)const;
	/// \brief Translation
	void translate(const Pt& pt);

	/// \brief Sub cloud access
	void get_range(StructuredPointCloud<Pt>& range,
		const uint32 * rect) const
	{
		get_range(range, rect[0], rect[1], rect[2], rect[3]);
	}

	/// \brief Sub matrix access
	void get_range(StructuredPointCloud<Pt>& range,
		uint32  x_min, uint32  y_min,
		uint32  nx, uint32  ny) const
	{
		container_t::get_range(range, x_min, y_min, nx, ny);
		range.timestamp(timestamp_);
	}
	/// \brief Assignment operator
	const StructuredPointCloud<Pt>& operator=
		(const StructuredPointCloud<Pt>& pts)
	{
		Clone(pts, *this);
		timestamp_ = pts.timestamp_;
		return *this;
	}
	/// \brief Count the number of valid points
	uint32 valid()const;
	/// \brief Invalidate some points according to
	/// an input matrix
	template<class Arr>
	void invalidate(const Arr&,
		typename Arr::value_type no_data = 0);


};
/// \brief Create with depth map
template<class Pt>
template<typename T>
void StructuredPointCloud<Pt>::create(
	uint32 w, uint32 h,
	T* depth,
	value_type scale,
	uint32 stride)
{
	container_t::create(h, w);
	if (scale == 1)
	{
		for (uint32 i = 0; i < h; ++i)
		{
			const T* ptr = reinterpret_cast<const T*>
				((const char*)depth + i * stride);
			for (uint32 j = 0; j < w; ++j)
				(*this)[i][j] = Pt(value_type(j),
								   value_type(i),
								   value_type(*ptr++));
		}
	}
	else
	{
		for (uint32 i = 0; i < h; ++i)
		{
			const T* ptr = reinterpret_cast<const T*>
				((const char*)depth + i * stride);
			for (uint32 j = 0; j < w; ++j)
				(*this)[i][j] = Pt(value_type(j),
									value_type(i),
									value_type((*ptr++)*scale));
		}
	}
}

/// \brief Create from unstructured point cloud
template<class Pt>
template<class Pts>
inline void StructuredPointCloud<Pt>::create
	(uint32 w, uint32 h, const Pts& pts)
{
	// Allocate
	container_t::create(h, w);
	fill(pts);
}
template<class Pt>
template<class Pts>
void StructuredPointCloud<Pt>::fill(const Pts& pts)
{
	// Copy data
	const typename Pts::point_type* ptr = pts.data();
	if (ptr != nullptr)
	{
		for (uint32 i = 0; i < container_t::size_x(); ++i)
		{
			for (uint32 j = 0; j < container_t::size_y(); ++j)
				(*this)[i][j] = Pt(*ptr++);
		}
	}
	// Copy timestamp
	timestamp_ = pts.timestamp();
}

// Is Empty
template<class Pt>
bool StructuredPointCloud<Pt>::empty()const
{
    bool res = true;
    for(uint32 i = 0;i < container_t::size_x(); ++i)
    {
        for(uint32 j = 0;j < container_t::size_y();++j)
        {
			if ((*this)[i][j].valid())
            {
                res = false;
                break;
            }
        }
        if(!res)
            break;
    }
    return res;
}

// Bounding box
template<class Pt>
uint32 StructuredPointCloud<Pt>::size()const
{
    uint32 count = 0;
    for(uint32 i = 0;i < container_t::size_x();++i)
    {
        for(uint32 j = 0;j < container_t::size_y();++j)
        {
			if ((*this)[i][j].valid())
                count++;
        }
    }
    return count;
}

// Bounding box
template<class Pt>
bool StructuredPointCloud<Pt>::bounding_box
    (Box<value_type,ndim>& box)const
{
    for(uint32 i = 0;i < container_t::size_x();++i)
    {
        for(uint32 j = 0;j < container_t::size_y();++j)
        {
            const Pt& pt = (*this)[i][j];
			if (pt.valid())
            {
                for (int32 i = 0; i < ndim; ++i)
                {
                    if (pt[i] < box.position_min[i])
                        box.position_min[i] = pt[i];

                    if (pt[i] > box.position_max[i])
                        box.position_max[i] = pt[i];
                }
            }
        }
    }
    return  empty()? false : true;
}
// Shortest distance
template<class Pt>
typename Pt::value_type StructuredPointCloud<Pt>::distance
(const Pt& pt)const
{
	value_type dist = Limit<value_type>::greatest();
	for (uint32 i = 0; i < container_t::size_x(); ++i)
		for (uint32 j = 0; j < container_t::size_y(); ++j)
		{
			const Pt& pti = (*this)[i][j];
			if (pti.valid())
			{
				value_type d = QuadNorm(pti - pt);
				if (d < dist)
					dist = d;
			}
		}
	if (dist < Limit<value_type>::greatest())
		dist = sqrt(dist);
	return dist;
}

// Shortest distance
template<class Pt>
typename Pt::value_type StructuredPointCloud<Pt>::distance
(const Pt& origin, const Pt& direction, value_type epsilon)const
{
	value_type dist = Limit<value_type>::greatest();
	Pt dir = direction;
	dir /= Norm(dir);
	epsilon *= epsilon;
	for (uint32 i = 0; i < container_t::size_x(); ++i)
		for (uint32 j = 0; j < container_t::size_y(); ++j)
		{
			const Pt& pti = (*this)[i][j];
			if (pti.valid())
			{
				pti -= origin;
				value_type d = Norm(Cross(pti, dir));
				if (d < epsilon)
				{
					d = Prod(pti, dir);
					if (Abs(d) < Abs(dist))
						dist = d;
				}
			}
		}
	return dist;
}

// Bounding box
template<class Pt>
inline void StructuredPointCloud<Pt>::translate
(const Pt& pt)
{
	for (uint32 i = 0; i < container_t::size_x(); ++i)
		for (uint32 j = 0; j < container_t::size_y(); ++j)
			if ((*this)[i][j].valid())
				(*this)[i][j] += pt;
}

// Count the number of valid points
template<class Pt>
uint32 StructuredPointCloud<Pt>::valid()const
{
	uint32 count = 0;
	for (uint32 i = 0; i < container_t::size_x(); ++i)
		for (uint32 j = 0; j < container_t::size_y(); ++j)
			if ((*this)[i][j].valid())
				count++;
	return count;
}
// Invalidate some points according to
// an input matrix
template<class Pt>
template<class Arr>
void StructuredPointCloud<Pt>::invalidate(const Arr& arr,
	typename Arr::value_type no_data)
{
	const uint32 sx = arr.size_x() < container_t::size_x() ?
		arr.size_x() : container_t::size_x();
	const uint32 sy = arr.size_y() < container_t::size_y() ?
		arr.size_y() : container_t::size_y();

	for (uint32 i = 0; i < sx; ++i)
		for (uint32 j = 0; j < sy; ++j)
			if (arr[i][j] == no_data)
				(*this)[i][j].invalidate();
}
} // core
#endif
