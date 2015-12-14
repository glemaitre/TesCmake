/// \file matrix.h
/// \brief Matrix class
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date August 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef CORE_MATRIX_H
#define CORE_MATRIX_H

#include "typedef.h"
#include <memory>

namespace core
{
template<typename T>
struct Allocator
{
	typedef typename std::allocator<T>::
		reference reference;
	typedef typename std::allocator<T>::
		pointer pointer;
	typedef typename std::allocator<T>::
		difference_type difference_type;
	typedef typename std::allocator<T>::
		value_type value_type;
	typedef value_type storage_type;
};

/// \brief Class Matrix
template<typename T,class Alloc= Allocator<T> >
class Matrix
{
public:
	typedef typename Alloc::difference_type difference_type;
	typedef typename Alloc::pointer pointer;
	typedef typename Alloc::reference reference;
	typedef typename Alloc::value_type value_type;
	typedef typename Alloc::storage_type storage_type;
    
	typedef typename std::shared_ptr<storage_type> Storage;
    typedef T type;

	enum {n_channels = 1};
private:
	pointer* last_;
protected:

	uint32 nx_; /// < Size x
	uint32 ny_; /// < Size y
	Storage data_storage_; /// < data storage
	pointer* data_access_; /// < Access to data
public:
	/// \brief Constructor
	Matrix():
		nx_(0),
		ny_(0),
		data_storage_(nullptr),
		data_access_(nullptr)
		{}

	/// \brief Constructor
	Matrix(uint32 nx,uint32 ny,
		const Storage& data = nullptr,
		const void* access = nullptr,
		uint32 stride = 0) :
		data_storage_(nullptr),
		data_access_(nullptr)
	{
		init(nx, ny, data, access,stride);
	}

	/// \brief Copy constructor
	Matrix(const Matrix& arr)
	{
		data_access_ = nullptr;
		data_storage_ = nullptr;
	    this->init(arr);
    }

	/// \brief Assignment operator
	Matrix& operator=(const Matrix& arr)
	{
		init(arr);
		return *this;
	}

	/// \brief Destructor
	virtual ~Matrix()
	{
		delete[] data_access_;
		data_storage_ = nullptr;
	}
	/// \brief Initialization
	void init(uint32 nx = 0,uint32  ny = 0);

	/// \brief Initialization
	void init(const Matrix& arr)
	{
		clear();
		init(arr.size_x(),arr.size_y());
		assign(arr);
	}

	/// \brief Initialization with Storage
	void init(uint32  nx,uint32  ny,
		const Storage& data,
		const void* access = nullptr,
		uint32  stride = 0)
	{
		clear();
		init(nx,ny);
		assign(data, access,stride);
	}
	/// \brief Reshape matrix
	void reshape(uint32  nx,uint32  ny)
	{
		assert(nx*ny <= nx_*ny_);
		init(nx, ny, data_storage_, data());
	}

	/// \brief Matrix assignation
	void assign(const Matrix& arr)
	{
		assign
			(arr.data_storage_,	// Storage
			&*arr.data(), // Access
			uint32 ((char*)(&*arr.data_access_[1])-
			(char*)(&*arr.data_access_[0])));  // Step
	}

	/// \brief Data access construction
	void assign(const Storage& data,
		const void* access = nullptr,
		uint32  stride = 0);

	/// \brief Allocation de mémoire
	void create()
	{
		Storage data = Storage(new storage_type[nx_*ny_], 
			[](storage_type* t){delete[] t; });
		assign(data,&*data);
	}

	/// \brief Allocation de la mémoire
	void create(uint32  nx,uint32  ny)
	{
		clear();
		init(nx,ny);
		create();
	}

	/// \brief Clear data
	void clear()
	{
        data_storage_ = nullptr;
        nx_ = 0;
        ny_ = 0;
    }

	/// \brief First element
	const pointer data()const
	{
		return (data_access_ != nullptr) ?
            data_access_[0] : nullptr;
	}
	pointer data()
	{
		return (data_access_ != nullptr) ?
			data_access_[0] : nullptr;
	}
	/// \brief Access operator
	const pointer& operator[](const difference_type i)const
	{
		assert(data_access_!= nullptr);
		assert(uint32 (i) < nx_);
		return data_access_[i];
	}

	pointer& operator[](const difference_type i)
	{
		assert(data_access_ != nullptr);
		assert(uint32(i) < nx_);
		return data_access_[i];
	}

    /// \brief Parenthesis operator
    const value_type& operator()(uint32 i,uint32 j)const
    {
        return (*this)[i][j];
    }
    /// \brief Parenthesis operator
    value_type& operator()(uint32 i,uint32 j)
    {
        return (*this)[i][j];
    }
	/// \brief Const access
	const pointer*& access() const
	{
		return data_access_;
	}

	/// \brief Access
	pointer*& access()
	{
		return data_access_;
	}

	/// \brief size x
	uint32  size_x()const
	{
		return nx_;
	}

	/// \brief size y
	uint32  size_y()const
	{
		return ny_;
	}

	/// \brief N bands
	uint32  n_bands() const
	{
		return n_channels;
	}
	/// \brief Stride
	uint32 stride()const
	{
		return nx_ > 0 ?
			uint32((char*)(&*data_access_[1]) -
				(char*)(&*data_access_[0])) : 0;
	}
	/// \brief Access to the storage
	const Storage& data_storage()const
	{
		return data_storage_;
	}

    bool empty()const
    {
        return size() == 0;
    }
	/// \brief Size (x*y)
	uint32  size()const
	{
		return nx_ * ny_;
	}
    /// \brief Sub matrix access
	void get_range(Matrix<T,Alloc>& range,const uint32 * rect) const;

    /// \brief Sub matrix access
	void get_range(Matrix<T,Alloc>& range,uint32  x_min,uint32  y_min,
        uint32  nx,uint32  ny) const;
};

template<typename T,class Alloc>
void Matrix<T,Alloc>::init
(uint32  nx,uint32  ny)
{
	nx_ = nx;
	ny_ = ny;
	// data_access creation
	if (data_access_ != nullptr)
	{
		if((last_-data_access_)< difference_type(nx))
		{
			delete[] data_access_;
			data_access_ = new pointer[nx_];
			last_ = data_access_+nx_;
		}
		if(data_storage_ != nullptr)
		{
			uint32  stride = ny_*sizeof(T);
			char* access = (char*)&*data_storage_;
			for(uint32  i = 0;i < nx_;i++)
				data_access_[i] = pointer(access + i * stride);
		}
	}
	else
	{
		data_access_ = new pointer[nx_];
		last_ = data_access_+nx_;
	}
}
// Storage assignation
template<typename T,class Alloc>
inline void Matrix<T,Alloc>::assign
(
	const Storage& data,
	const void* access,
	uint32  stride
)
{
	if(data != nullptr)
		data_storage_ = data;
	else
		data_storage_ = Storage(new storage_type[1]);

	stride = (stride == 0) ? ny_*sizeof(T) : stride ;
	assert( size_t(stride) >= sizeof(T)* ny_);
	access = access == nullptr ? &*data_storage_ : access;
	for(uint32  i = 0;i < nx_;i++)
		data_access_[i] = pointer((char*) access + i * stride);
}

// Sub Matrix access
template<typename T,class Alloc>
inline void Matrix<T,Alloc>::get_range(Matrix<T,Alloc>& range,
									   const uint32 * rect)const
{
    get_range(range,rect[0],rect[1],rect[2],rect[3]);
}

// Sub matrix access
template<typename T,class Alloc>
inline void Matrix<T,Alloc>::get_range(Matrix<T,Alloc>& range,
    uint32  x_min,uint32  y_min,
    uint32  nx,uint32  ny) const
{
	assert(x_min + nx <= nx_);
	assert(y_min + ny <= ny_);

	uint32  stride = this->stride();
	char* access  = (char*)(&*data()) + y_min * sizeof(T);
	access += x_min * stride;
	range.init(nx, ny, data_storage_, access, stride);
}

template<typename T, class Alloc>
inline void Diagonal(Matrix<T,Alloc>& mat, const T& val)
{
    uint32  nx = mat.size_x();
    uint32  ny = mat.size_y();
    uint32  m = nx < ny ? nx:ny;

	Fill(mat,T(0));
	for(uint32  i = 0; i < m;++i)
        mat[i][i] = val;
}
template<typename T, class Alloc>
inline void Fill(Matrix<T,Alloc>& mat, const T& val)
{
	assert(mat.data_storage() != nullptr);
	for(uint32  i = 0; i < mat.size_x();++i)
		for(uint32  j = 0;j < mat.size_y();++j)
			mat[i][j] = val;
}

// Matrix copy
template<typename T, class Alloc>
inline void Copy(const Matrix<T, Alloc>&  src, Matrix<T, Alloc>&  dst, 
	const T& default_val  = T(0))
{
	assert((src.size_x() <= dst.size_x())
		&& (src.size_y() <= dst.size_y()));
	assert(src.data_storage() != nullptr);
	assert(dst.data_storage() != nullptr);

	for (uint32 i = 0; i < src.size_x(); ++i)
	{
		for (uint32 j = 0; j < src.size_y(); ++j)
			dst[i][j] = src[i][j];
		for (uint32 j = src.size_y(); j < dst.size_y(); j++)
			dst[i][j] = default_val;
	}
	for (uint32 i = src.size_x(); i < dst.size_x(); ++i)
		for (uint32 j = 0; j < dst.size_y(); ++j)
			dst[i][j] = default_val;
}

//Matrix clone (memory allocation and copy)
template<typename T, class Alloc>
inline void Clone(const Matrix<T, Alloc>&  src, Matrix<T, Alloc>&  dst)
{
	assert(src.data_storage() != nullptr);
	dst.init(src.size_x(), src.size_y());
	dst.create();
	Copy(src, dst);
}

} //core
#endif
