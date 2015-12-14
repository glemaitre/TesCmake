/// \file smallmatrix.h
/// \brief Small Matrix class
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date August 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef CORE_SMALL_MATRIX_H
#define CORE_SMALL_MATRIX_H

#include "typedef.h"
#include "string.h"
#include "smallmatrixfnct.h"

namespace core
{

// SmallMatrix declaration
template<typename T,int32 I,int32 J>
class SmallMatrix
{
private:
	T ptr_[I][J]; /// < Data
public:
	typedef T value_type;
	typedef SmallMatrix<T, I, J> mat_t;
	enum{ nelem = I * J };
	/// \brief Constructeur
	SmallMatrix(){};
	explicit SmallMatrix(const T value)
	{
		Fill(*this,value);
	}
	/// \brief Constructor
	SmallMatrix(const T* val)
	{
		*this = val;
	}
	/// \brief Set all values to zero
	void clear()
	{
		Fill(*this);
	}
	/// \brief Initialization
	void init()
	{
		Identity(*this);
	}
	/// \brief Assignment operator
	const mat_t& operator= (const mat_t& src)
	{
		memcpy(ptr_[0], src.ptr_[0], sizeof(T)*I*J);
		return *this;
	}
	/// \brief Assignment operator with ptr
	const mat_t& operator= (T* src)
	{
		memcpy(ptr_[0], src, sizeof(T)*I*J);
		return *this;
	}
	/// \brief Access to data
	T* operator[](int32 k)
	{
		assert(k < I);
		return ptr_[k];
	}
	/// \brief Const access to data
	const T* operator[](int32 k) const
	{
		assert(k < I);
		return ptr_[k];
	}
	/// \brief X size
	CONSTEXPR int32 size_x() const
	{
		return I;
	}
	/// \brief Y size
	CONSTEXPR int32 size_y() const
	{
		return J;
	}
	/// \brief Size
	CONSTEXPR int32 size()const
	{
		return nelem;
	}
	/// \brief Pointer access
	const T* data()const
	{
		return ptr_[0];
	}
	T* data()
	{
        return ptr_[0];
	}
};

/// \brief Fill diagonal entries
template<typename T, int32 I, int32 J>
void Diag(SmallMatrix<T, I, J>& mat, const T& val)
{
    MatFill<I, J>::apply(mat, T(0));
	MatDiag<I, J>::apply(mat,val);
}
/// \brief Fill the matrix
template<typename T, int32 I, int32 J>
void Fill(SmallMatrix<T, I, J>& mat, const T& val = T(0))
{
	MatFill<I, J>::apply(mat, val);
}
/// \brief Create an indentity matrix
template<typename T, int32 I, int32 J>
void Identity(SmallMatrix<T, I, J>& mat,
	const T& val=T(1))
{
	MatFill<I, J>::apply(mat, T(0));
	MatDiag<I, J>::apply(mat, val);
}

/// \brief Addition
template<typename T, int32 I, int32 J>
inline void Add(const SmallMatrix<T, I, J>& src1,
	const SmallMatrix<T, I, J>& src2,
	SmallMatrix<T, I, J>& dst)
{
	MatAdd<I, J>::apply(src1, src2, dst);
}
template<typename T, int32 I, int32 J>
inline void Add(const SmallMatrix<T, I, J>& src1,
	SmallMatrix<T, I, J>& dst)
{
	MatAdd<I, J>::apply(src1, dst);
}
/// \brief Const value addition
template<typename T, int32 I, int32 J>
inline void AddVal(const SmallMatrix<T, I, J>& src1,
	SmallMatrix<T, I, J>& dst, const T& value)
{
	MatAddVal<I, J>::apply(src1, dst, value);
}
template<typename T, int32 I, int32 J>
inline void AddVal(SmallMatrix<T, I, J>& dst, const T& value)
{
	MatAddVal<I, J>::apply(dst, value);
}

/// \brief Substraction
template<typename T, int32 I, int32 J>
inline void Sub(const SmallMatrix<T, I, J>& src1,
	const SmallMatrix<T, I, J>& src2,
	SmallMatrix<T, I, J>& dst)
{
	MatSub<I, J>::apply(src1, src2, dst);
}
template<typename T, int32 I, int32 J>
inline void Sub(const SmallMatrix<T, I, J>& src1,
	SmallMatrix<T, I, J>& dst)
{
	MatSub<I, J>::apply(src1, dst);
}

/// \brief Const value substraction
template<typename T, int32 I, int32 J>
inline void SubVal(const SmallMatrix<T, I, J>& src1,
	SmallMatrix<T, I, J>& dst, const T& value)
{
	MatAddVal<I, J>::apply(src1, dst, -value);
}

template<typename T, int32 I, int32 J>
inline void SubVal(SmallMatrix<T, I, J>& dst, const T& value)
{
	MatAddVal<I, J>::apply(dst, -value);
}

/// \brief Multiplication
template<typename T, int32 I, int32 J>
inline void Mult(const SmallMatrix<T, I, J>& src1,
	const SmallMatrix<T, I, J>& src2,
	SmallMatrix<T, I, J>& dst)
{
	MatMult<I, J>::apply(src1, src2, dst);
}
template<typename T, int32 I, int32 J>
inline void Mult(const SmallMatrix<T, I, J>& src1,
	SmallMatrix<T, I, J>& dst)
{
	MatMult<I, J>::apply(src1, dst);
}
/// \brief Const multiplication
template<typename T, int32 I, int32 J>
inline void MultVal(const SmallMatrix<T, I, J>& src1,
	SmallMatrix<T, I, J>& dst, const T& value)
{
	MatMultVal<I, J>::apply(src1, dst, value);
}
template<typename T, int32 I, int32 J>
inline void MultVal(SmallMatrix<T, I, J>& dst, const T& value)
{
	MatMultVal<I, J>::apply(dst, value);
}
/// \brief Division
template<typename T, int32 I, int32 J>
inline void Div(const SmallMatrix<T, I, J>& src1,
	const SmallMatrix<T, I, J>& src2, SmallMatrix<T, I, J>& dst)
{
	MatDiv<I, J>::apply(src1, src2, dst);
}
template<typename T, int32 I, int32 J>
inline void Div(const SmallMatrix<T, I, J>& src1, SmallMatrix<T, I, J>& dst)
{
	MatDiv<I, J>::apply(src1, dst);
}
/// \brief Const division
template<typename T, int32 I, int32 J>
inline void DivVal(const SmallMatrix<T, I, J>& src1,
	SmallMatrix<T, I, J>& dst, const T& value)
{
	MatMultVal<I, J>::apply(src1, dst, 1 / value);
}
template<typename T, int32 I, int32 J>
inline void DivVal(SmallMatrix<T, I, J>& dst, const T& value)
{
	MatMultVal<I, J>::apply(dst, 1 / value);
}
/// \brief Copy
template<typename T, int32 I, int32 J>
inline void Copy(const SmallMatrix<T, I, J>& src, SmallMatrix<T, I, J>& dst)
{
	dst = src;
}
/// \brief Transpose
template<typename T, int32 I, int32 J>
inline void Transpose(const SmallMatrix<T, I, J>& src, SmallMatrix<T, J, I>& dst)
{
	assert((void*)&src != (void*)&dst);
	MatTranspose<I, J>::apply(src, dst);
}
/// \brief Matrix product
template<typename T, int32 I, int32 J, int32 K>
inline void Prod
(const SmallMatrix<T, I, J>& src1, const SmallMatrix<T, J, K>& src2, SmallMatrix<T, I, K>& dst)
{
	MatProd<I, J, K>::apply(src1, src2, dst);
}
template<typename T, int32 I, int32 J, int32 K>
inline void TranspProd
(const SmallMatrix<T, J, I>& src1, const SmallMatrix<T, J, K>& src2, SmallMatrix<T, I, K>& dst)
{
	MatTranspProd<I, J, K>::apply(src1, src2, dst);
}
template<typename T, int32 I, int32 J, int32 K>
inline void ProdTransp
(const SmallMatrix<T, I, J>& src1, const SmallMatrix<T, K, J>& src2, SmallMatrix<T, I, K>& dst)
{
	MatProdTransp<I, J, K>::apply(src1, src2, dst);
}

/// \brief Matrix Product A x Vector
template<typename T, int32 I, int32 J, class V1, class V2>
inline void ProdVec
(const SmallMatrix<T, I, J>& mat, const V1& vec, V2& dst)
{
	MatProdVec<I, J>::apply(mat,vec, dst);
}

/// \brief Matrix product At x Vector
template<typename T, int32 I, int32 J,class V1,class V2>
inline void TranspProdVec
(const SmallMatrix<T, I, J>& mat,const V1& vec, V2& dst)
{
	MatTranspProdVec<I, J>::apply(mat,vec, dst);
}

////////////////////////////////////////////
// Binary operations
/// \brief Addition
template<typename L,typename R,int32 I,int32 J>
inline SmallMatrix<decltype(L()+R()),I,J>
    operator+ (const SmallMatrix<L,I,J>& mat_l,
    const SmallMatrix<R,I,J>& mat_r)
{
    SmallMatrix<decltype(L()+R()),I,J> dst;
    Add(mat_l,mat_r,dst);
    return dst;
}

template<typename L,typename R,int32 I,int32 J>
inline SmallMatrix<decltype(L()+R()),I,J>
    operator+ (const SmallMatrix<L,I,J>& mat_l,const R& value)
{
    SmallMatrix<decltype(L()+R()),I,J> dst;
    AddVal(mat_l,dst,value);
    return dst;
}
template<typename L,typename R,int32 I,int32 J>
inline SmallMatrix<decltype(L()+R()),I,J>
    operator+ (const L& value,const SmallMatrix<R,I,J> mat_r)
{
    SmallMatrix<decltype(L()+R()),I,J> dst;
    AddVal(mat_r,dst,value);
    return dst;
}
/// \brief Substraction
template<typename L,typename R,int32 I,int32 J>
inline SmallMatrix<decltype(L()-R()),I,J>
    operator- (const SmallMatrix<L,I,J>& mat_l,
    const SmallMatrix<R,I,J>& mat_r)
{
    SmallMatrix<decltype(L()-R()),I,J> dst;
    Sub(mat_l,mat_r,dst);
    return dst;
}
template<typename L,typename R,int32 I,int32 J>
inline SmallMatrix<decltype(L()-R()),I,J>
    operator- (const SmallMatrix<L,I,J>& mat_l,const R& value)
{
    SmallMatrix<decltype(L()-R()),I,J> dst;
    SubVal(mat_l,dst,value);
    return dst;
}
template<typename L,typename R,int32 I,int32 J>
inline SmallMatrix<decltype(L()-R()),I,J>
    operator- (const L& value,const SmallMatrix<R,I,J> mat_r)
{
    SmallMatrix<decltype(L()-R()),I,J> dst;
    SmallMatrix<L,I,J> mat_l(value);
    Sub(mat_l,mat_r,dst);
    return dst;
}
/// \brief Multiplication
template<typename L,typename R,int32 I,int32 J>
inline SmallMatrix<decltype(L()*R()),I,J>
    operator* (const SmallMatrix<L,I,J>& mat_l,
    const SmallMatrix<R,I,J>& mat_r)
{
    SmallMatrix<decltype(L()*R()),I,J> dst;
    Mult(mat_l,mat_r,dst);
    return dst;
}
template<typename L,typename R,int32 I,int32 J>
inline SmallMatrix<decltype(L()*R()),I,J>
    operator* (const SmallMatrix<L,I,J>& mat_l,const R& value)
{
    SmallMatrix<decltype(L()*R()),I,J> dst;
    MultVal(mat_l,dst,value);
    return dst;
}
template<typename L,typename R,int32 I,int32 J>
inline SmallMatrix<decltype(L()*R()),I,J>
    operator* (const L& value,const SmallMatrix<R,I,J> mat_r)
{
    SmallMatrix<decltype(L()*R()),I,J> dst;
    MultVal(mat_r,dst,value);
    return dst;
}
/// \brief Division
template<typename L,typename R,int32 I,int32 J>
inline SmallMatrix<decltype(L()/R()),I,J>
    operator/ (const SmallMatrix<L,I,J>& mat_l,
    const SmallMatrix<R,I,J>& mat_r)
{
    SmallMatrix<decltype(L()/R()),I,J> dst;
    Div(mat_l,mat_r,dst);
    return dst;
}
template<typename L,typename R,int32 I,int32 J>
inline SmallMatrix<decltype(L()/R()),I,J>
    operator/ (const L& value,const SmallMatrix<R,I,J>& mat_r)
{
    SmallMatrix<decltype(L()/R()),I,J> dst;
    SmallMatrix<L,I,J> mat_l(value);
    Div(mat_l,mat_r,dst);
    return dst;
}
template<typename L,typename R,int32 I,int32 J>
inline SmallMatrix<decltype(L()/R()),I,J>
    operator/ (const SmallMatrix<L,I,J>& mat_l,const R& value)
{
    SmallMatrix<decltype(L()*R()),I,J> dst;
    DivVal(mat_l,dst,value);
    return dst;
}
////////////////////////////////////////////
// Unary operations
/// \brief Addition
template<typename L,typename R,int32 I,int32 J>
inline void operator+= (SmallMatrix<L,I,J>& dst,
    const SmallMatrix<R,I,J>& vec)
{
    Add(vec,dst);
}

template<typename L,typename R,int32 I,int32 J>
inline void operator += (SmallMatrix<L,I,J>& dst,const R& value)
{
    AddVal(dst,value);
}
/// \brief Soustraction
template<typename L,typename R,int32 I,int32 J>
inline void operator-= (SmallMatrix<L,I,J>& dst,
    const SmallMatrix<R,I,J>& vec)
{
    Sub(vec,dst);
}
template<typename L,typename R,int32 I,int32 J>
inline void
    operator-= (SmallMatrix<L,I,J>& dst,const R& value)
{
    SubVal(dst,value);
}

/// \brief Multiplication
template<typename L,typename R,int32 I,int32 J>
inline void
    operator*= (SmallMatrix<L,I,J>& dst,
    const SmallMatrix<R,I,J>& vec)
{
    Mult(vec,dst);
}
template<typename L,typename R,int32 I,int32 J>
inline void
    operator*= (SmallMatrix<L,I,J>& dst,const R& value)
{
    MultVal(dst,value);
}
/// \brief Division
template<typename L,typename R,int32 I,int32 J>
inline void operator/= (SmallMatrix<L,I,J>& dst,
    const SmallMatrix<R,I,J>& vec)
{
    Div(vec,dst);
}
template<typename L,typename R,int32 I,int32 J>
inline void operator/= (SmallMatrix<L,I,J>& dst,const R& value)
{
    DivVal(dst,value);
}
} // core

#endif
