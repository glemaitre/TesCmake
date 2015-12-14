/// \file smallvector.h
/// \brief Small Vector class
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date August 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef CORE_SMALL_VECTOR_H
#define CORE_SMALL_VECTOR_H

#include "smallvectorfnct.h"
#include "typedef.h"
#include "math.h"

namespace core
{
	template<typename T, int32 I>
	struct SmallVector
	{
	public:
        typedef T value_type;
		enum{ nelem = I };
	protected:
		T data_[nelem]; /// > Data
	public:
		/// \brief Default constructor
		SmallVector(){}
		/// \brief Constructor
		SmallVector(const T* st)
		{
			*this = st;
		}
		SmallVector(std::initializer_list<T> il);

		/// \brief Copy constructor
		SmallVector(const SmallVector& st)
		{
			*this = st;
		}

		explicit SmallVector(const T& val)
		{
			Fill(*this, val);
		}
		/// \brief Set all values to zero
		void clear()
		{
			init();
		}
		/// \brief Initialization
		void init()
		{
			Fill(*this);
		}
		/// \brief Access operator
		T& operator[](int32 i)
		{
			assert(i < nelem);
			return data_[i];
		}
		/// \brief Const access operator
		const T& operator[](int32 i)const
		{
			assert(i < nelem);
			return data_[i];
		}
		/// \brief Assignment operator
		template<int32 J>
		const SmallVector<T, I>& operator=(const SmallVector<T, J>& st)
		{
			assert(I <= J);
			VecCopy<I>::apply(st.data_,this->data_);
			return *this;
		}
		/// \brief Assignment operator with pointer
		const SmallVector<T, I>& operator=(const T* st)
		{
			VecCopy<I>::apply(st, this->data_);
			return *this;
		}
		/// \brief Assignment operator with pointer
		const SmallVector<T, I>& operator=(const T& val)
		{
            VecFill<I>::apply(this->data_,val);
			return *this;
		}
		/// \brief const access to data pointer
		const T* data()const
		{
			return data_;
		}
		T* data()
		{
			return data_;
		}
		/// \brief Size
		CONSTEXPR int32 size()const
		{
			return nelem;
		}
	};
	template<typename T,int32 I>
	SmallVector<T, I>::SmallVector(std::initializer_list<T> il)
	{
		assert(il.size() >= nelem);
		auto iter = il.begin();
		for (int32 i = 0; i < nelem; ++i)
			data_[i] = *iter++;
	}
	/// \brief Fill the vector with const value
	template<typename T, int32 I>
	inline void Fill(SmallVector<T, I>& vec,
		const T& val = T(0))
	{
		VecFill<I>::apply(vec,val);
	}
	/// \brief Vector addtion
	template<typename T1,typename T , int32 I>
	inline void Add(const SmallVector<T1, I>& src1,
		SmallVector<T, I>& dst)
	{
		VecAdd<I>::apply(src1, dst);
	}
	template<typename T1,typename T2,typename T , int32 I>
	inline void Add(const SmallVector<T1, I>& src1,
		const SmallVector<T2, I>& src2,
		SmallVector<T, I>& dst)
	{
		VecAdd<I>::apply(src1,src2, dst);
	}
	/// \brief Vector substraction
	template<typename T1,typename T , int32 I>
	inline void Sub(const SmallVector<T1, I>& src1,
		SmallVector<T, I>& dst)
	{
		VecSub<I>::apply(src1, dst);
	}
	template<typename T1,typename T2,typename T , int32 I>
	inline void Sub(const SmallVector<T1, I>& src1,
		const SmallVector<T2, I>& src2,
		SmallVector<T, I>& dst)
	{
		VecSub<I>::apply(src1, src2, dst);
	}
	/// \brief Vector multiplication
	template<typename T1,typename T2,typename T , int32 I>
	inline void Mult(const SmallVector<T1, I>& src1,
		const SmallVector<T2, I>& src2,
		SmallVector<T, I>& dst)
	{
		VecMult<I>::apply(src1, src2, dst);
	}
	template<typename T1,typename T , int32 I>
	inline void Mult(const SmallVector<T1, I>& src1,
		SmallVector<T, I>& dst)
	{
		VecMult<I>::apply(src1, dst);
	}
	/// \brief Vector division
	template<typename T1,typename T2,typename T , int32 I>
	inline void Div(const SmallVector<T1, I>& src1,
		const SmallVector<T2, I>& src2,
		SmallVector<T, I>& dst)
	{
		VecDiv<I>::apply(src1, src2, dst);
	}
	template<typename T1,typename T , int32 I>
	inline void Div(const SmallVector<T1, I>& src1,
		SmallVector<T, I>& dst)
	{
		VecDiv<I>::apply(src1, dst);
	}
	/// \brief Const addition
	template<typename T1,typename T,typename V, int32 I>
	inline void AddVal(const SmallVector<T1, I>& src1,
		SmallVector<T, I>& dst, const V& value)
	{
		VecAddVal<I>::apply(src1, dst, value);
	}
	template<typename T,typename V, int32 I>
	inline void AddVal(SmallVector<T, I>& dst, const V& value)
	{
		VecAddVal<I>::apply(dst, value);
	}
	/// \brief Const substraction
	template<typename T1,typename T,typename V, int32 I>
	inline void SubVal(const SmallVector<T1, I>& src1,
		SmallVector<T, I>& dst, const V& value)
	{
		VecSubVal<I>::apply(src1, dst, value);
	}
	template<typename T,typename V, int32 I>
	inline void SubVal(SmallVector<T, I>& dst, const V& value)
	{
		VecSubVal<I>::apply(dst, value);
	}
	/// \brief Const multiplication
	template<typename T1,typename T,typename V, int32 I>
	inline void MultVal(const SmallVector<T1, I>& src1,
		SmallVector<T, I>& dst, const V& value)
	{
		VecMultVal<I>::apply(src1, dst, value);
	}
	template<typename T,typename V, int32 I>
	inline void MultVal(SmallVector<T, I>& dst, const V& value)
	{
		VecMultVal<I>::apply(dst, value);
	}
	/// \brief Const division
	template<typename T1,typename T,typename V, int32 I>
	inline void DivVal(const SmallVector<T1, I>& src1,
		SmallVector<T, I>& dst, const V& value)
	{
		VecDivVal<I>::apply(src1, dst, value);
	}
	template<typename T,typename V, int32 I>
	inline void DivVal(SmallVector<T, I>& dst, const V& value)
	{
		VecDivVal<I>::apply(dst, value);
	}
	/// \brief Copy
	template<typename T1,typename T,int32 I>
	inline void Copy(const SmallVector<T1, I>& src1,
		SmallVector<T, I>& dst)
	{
		VecCopy<I>::apply(src1, dst);
	}
	/// \brief Absolute value
	template<typename T1, typename T, int32 I>
	inline void Abs(const SmallVector<T1, I>& src1,
		SmallVector<T, I>& dst)
	{
		VecAbs<I>::apply(src1, dst);
	}
	template<typename T1, typename T, int32 I>
	inline void Abs(SmallVector<T1, I>& src1)
	{
		VecAbs<I>::apply(src1);
	}
	/// \brief Sign
	template<typename T1, typename T, int32 I>
	inline void Sign(const SmallVector<T1, I>& src1,
		SmallVector<T, I>& dst)
	{
		VecSign<I>::apply(src1, dst);
	}
	template<typename T1, int32 I>
	inline void Sign(SmallVector<T1, I>& src)
	{
		VecSign<I>::apply(src);
	}
	/// \brief Scalar product
	template<typename T1, typename T2,typename T, int32 I>
	inline void Prod(const SmallVector<T1, I>& src1,
		const SmallVector<T2, I>& src2,T& dst)
	{
		VecScalarProd<I>::apply(src1, src2, dst);
	}

	template<typename T, int32 I>
	inline T Prod(const SmallVector<T, I>& src1,
		const SmallVector<T, I>& src2)
	{
        T res;
		VecScalarProd<I>::apply(src1, src2, res);
		return res;
	}

	template<typename T1,typename T2,int32 I>
	inline decltype(T1()*T2()) Prod(const SmallVector<T1, I>& src1,
		const SmallVector<T2, I>& src2)
	{
		decltype(T1()*T2()) res;
		VecScalarProd<I>::apply(src1, src2, res);
		return res;
	}
    /// \brief Quadratic norm
    template<typename T,int32 I>
	inline T QuadNorm(const SmallVector<T, I>& vec)
    {
        T res;
		VecQuadNorm<I>::apply(vec, res);
        return res;
    };
	template<typename T, int32 I>
	inline float_t Norm(const SmallVector<T, I>& vec)
	{
		return sqrt(QuadNorm(vec));
	};

	/// \brief Cross product
	template<typename T1, typename T2>
	inline decltype(T1()*T2())  Cross(const SmallVector<T1, 2>& src1,
		const SmallVector<T2, 2>& src2)
	{
		decltype(T1()*T2()) res;
		VecCross2::apply(src1, src2, res);
		return res;
	}

	template<typename T1,typename T2,typename T>
	inline void Cross(const SmallVector<T1, 3>& src1,
		const SmallVector<T2, 3>& src2, SmallVector<T, 3>& dst)
	{
		VecCross3::apply(src1, src2, dst);
	}

    ////////////////////////////////////////////
    // Binary operations
    /// \brief Addition
    template<typename L,typename R,int32 I>
    inline SmallVector<decltype(L()+R()),I>
        operator+ (const SmallVector<L,I>& vec_l,
        const SmallVector<R,I>& vec_r)
    {
        SmallVector<decltype(L()+R()),I> dst;
        Add(vec_l,vec_r,dst);
        return dst;
    }

    template<typename T,int32 I>
    inline SmallVector<T,I>
        operator+ (const SmallVector<T,I>& vec_l,const T& value)
    {
        SmallVector<T,I> dst;
        AddVal(vec_l,dst,value);
        return dst;
    }
	template<typename T, int32 I>
	inline SmallVector<T, I>
		operator+ (const T& value, const SmallVector<T, I>& vec_r)
    {
		SmallVector<T, I> dst;
        AddVal(vec_r,dst,value);
        return dst;
    }
    /// \brief Soustraction
    template<typename L,typename R,int32 I>
    inline SmallVector<decltype(L()-R()),I>
        operator- (const SmallVector<L,I>& vec_l,
        const SmallVector<R,I>& vec_r)
    {
		SmallVector<L, I> dst;
        Sub(vec_l,vec_r,dst);
        return dst;
    }

	template<typename T, int32 I>
	inline SmallVector<T, I>
		operator- (const SmallVector<T, I>& vec_l, T value)
	{
		SmallVector<T, I> dst;
		SubVal(vec_l, dst, value);
		return dst;
	}

	template<typename T, int32 I>
	inline SmallVector<T, I>
		operator- (T value, const SmallVector<T, I>& vec_r)
	{
		SmallVector<T, I> dst;
		SmallVector<T, I> vec_l(value);
		Sub(vec_l, vec_r, dst);
		return dst;
	}
    /// \brief Multiplication
    template<typename L,typename R,int32 I>
    inline SmallVector<decltype(L()*R()),I>
        operator* (const SmallVector<L,I>& vec_l,
        const SmallVector<R,I>& vec_r)
    {
        SmallVector<decltype(L()*R()),I> dst;
        Mult(vec_l,vec_r,dst);
        return dst;
    }
	template<typename T, int32 I>
	inline SmallVector<T, I>
		operator* (const SmallVector<T, I>& vec_l, const T& value)
    {
		SmallVector<T, I> dst;
        MultVal(vec_l,dst,value);
        return dst;
    }
	template<typename T, int32 I>
	inline SmallVector<T, I>
		operator* (const T& value, const SmallVector<T, I>& vec_r)
    {
		SmallVector<T, I> dst;
        MultVal(vec_r,dst,value);
        return dst;
    }
    /// \brief Division
    template<typename L,typename R,int32 I>
    inline SmallVector<decltype(L()/R()),I>
        operator/ (const SmallVector<L,I>& vec_l,
        const SmallVector<R,I>& vec_r)
    {
        SmallVector<decltype(L()/R()),I> dst;
        Div(vec_l,vec_r,dst);
        return dst;
    }
	template<typename T, int32 I>
	inline SmallVector<T, I>
		operator/ (const T& value, const SmallVector<T, I>& vec_r)
    {
		SmallVector<T, I> dst;
		SmallVector<T, I> vec_l(value);
        Div(vec_l,vec_r,dst);
        return dst;
    }
	template<typename T, int32 I>
	inline SmallVector<T, I>
		operator/ (const SmallVector<T, I>& vec_l, const T& value)
    {
		SmallVector<T, I> dst;
        DivVal(vec_l,dst,value);
        return dst;
    }

    template<typename L,typename R,int32 I>
    inline bool operator== (const SmallVector<L,I>& vec_l,
            const SmallVector<R,I>& vec_r)
    {
        return VecEqual<I>::apply(vec_l,vec_r);
    }
    template<typename L,typename R,int32 I>
    inline bool operator!= (const SmallVector<L,I>& vec_l,
            const SmallVector<R,I>& vec_r)
    {
        return !VecEqual<I>::apply(vec_l,vec_r);
    }
    ////////////////////////////////////////////
    // Unary operations
    /// \brief Addition
    template<typename L,typename R,int32 I>
    inline void operator+= (SmallVector<L,I>& dst,
        const SmallVector<R,I>& vec)
    {
        Add(vec,dst);
    }

	template<typename T, int32 I>
	inline void operator += (SmallVector<T, I>& dst, const T& value)
    {
        AddVal(dst,value);
    }
    /// \brief Soustraction
    template<typename L,typename R,int32 I>
    inline void operator-= (SmallVector<L,I>& dst,
        const SmallVector<R,I>& vec)
    {
        Sub(vec,dst);
    }
	template<typename T, int32 I>
    inline void
		operator-= (SmallVector<T, I>& dst, const T& value)
    {

        SubVal(dst,value);
    }

    /// \brief Multiplication
    template<typename L,typename R,int32 I>
    inline void
        operator*= (SmallVector<L,I>& dst,
        const SmallVector<R,I>& vec)
    {
        Mult(vec,dst);
    }
	template<typename T, int32 I>
    inline void
		operator*= (SmallVector<T, I>& dst, const T& value)
    {
        MultVal(dst,value);
    }
    /// \brief Division
    template<typename L,typename R,int32 I>
    inline void operator/= (SmallVector<L,I>& dst,
        const SmallVector<R,I>& vec)
    {
        Div(vec,dst);
    }
	template<typename T, int32 I>
	inline void operator/= (SmallVector<T, I>& dst, const T& value)
    {
        DivVal(dst,value);
    }
} // core
#endif
