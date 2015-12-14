/// \file smallvectorfnct.h
/// \brief Small Vector functions
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date August 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef CORE_SMALL_VECTOR_FUNCTION_H
#define CORE_SMALL_VECTOR_FUNCTION_H

#include "typedef.h"
#include "abs.h"
#include "type_traits"

namespace core
{
	/////////////////////////////////////////////////////////
	/// \brief Fill the matrix with a const value

	// Declaration
	template<int32 I, int32 COUNT_I = 0>
	struct VecFill;

	// General case
	template<int32 I,int32 COUNT_I>
	struct VecFill
	{
		template<class Vec, typename T>
		static void apply(Vec& src, const T& value)
		{
			src[COUNT_I] = value;
			VecFill<I,COUNT_I+1>::apply(src, value);
		}
	};

	// End
	template<int32 I>
	struct VecFill<I,I>
	{
		template<class Vec, typename T>
		static void apply(Vec& src, const T& value){}
	};
	////////////////////////////////////////////////////
	/// \brief Vector addition

	// Declaration
	template<int32 I, int32 COUNT_I = 0>
	struct VecAdd;

	// General case
	template<int32 I, int32 COUNT_I>
	struct VecAdd
	{
		template<class V1, class V2, class V3>
		static void apply(const V1& src1, const V2& src2, V3& dst)
		{
            typedef typename std::remove_reference<decltype(dst[0])>::type T;
			dst[COUNT_I] = T(src1[COUNT_I] + src2[COUNT_I]);
			VecAdd<I, COUNT_I + 1>::apply(src1, src2, dst);
		}
		template<class V1, class V2>
		static void apply(const V1& src1, V2& dst)
		{
			dst[COUNT_I] += src1[COUNT_I];
			VecAdd<I, COUNT_I + 1>::apply(src1, dst);
		}
	};

	// End
	template<int32 I>
	struct VecAdd<I, I>
	{
		template<class V1, class V2, class V3>
		static void apply(const V1&, const V2&, V3&){}

		template<class V1, class V2>
		static void apply(const V1&, V2&){}
	};
	////////////////////////////////////////////////////
	/// \brief Vector substraction

	// Gereral case
	template<int32 I, int32 COUNT_I = 0>
	struct VecSub;

	// General case
	template<int32 I, int32 COUNT_I>
	struct VecSub
	{
		template<class V1, class V2, class V3>
		static void apply(const V1& src1, const V2& src2, V3& dst)
		{
            typedef typename std::remove_reference<decltype(dst[0])>::type T;
			dst[COUNT_I] = T(src1[COUNT_I] - src2[COUNT_I]);
			VecSub<I, COUNT_I + 1>::apply(src1, src2, dst);
		}
		template<class V1, class V2>
		static void apply(const V1& src1, V2& dst)
		{
			dst[COUNT_I] -= src1[COUNT_I];
			VecSub<I, COUNT_I + 1>::apply(src1, dst);
		}
	};
	// End
	template<int32 I>
	struct VecSub<I, I>
	{
		template<class V1, class V2, class V3>
		static void apply(const V1&, const V2&, V3&){}

		template<class V1, class V2>
		static void apply(const V1&, V2&){}
	};

	////////////////////////////////////////////////////
	/// \brief Vector multiplication

	// Declaration
	template<int32 I, int32 COUNT_I = 0>
	struct VecMult;

	// General case
	template<int32 I, int32 COUNT_I>
	struct VecMult
	{
		template<class V1, class V2, class V3>
		static void apply(const V1& src1, const V2& src2, V3& dst)
		{
            typedef typename std::remove_reference<decltype(dst[0])>::type T;
			dst[COUNT_I] = T(src1[COUNT_I] * src2[COUNT_I]);
			VecMult<I, COUNT_I + 1>::apply(src1, src2, dst);
		}
		template<class V1, class V2>
		static void apply(const V1& src1, V2& dst)
		{
			dst[COUNT_I] *= src1[COUNT_I];
			VecMult<I, COUNT_I + 1>::apply(src1, dst);
		}
	};
	// End
	template<int32 I>
	struct VecMult<I, I>
	{
		template<class V1, class V2, class V3>
		static void apply(const V1&, const V2&, V3&){}

		template<class V1, class V2>
		static void apply(const V1&, V2&){}
	};

	////////////////////////////////////////////////////
	/// \brief Vector division
	template<int32 I, int32 COUNT_I = 0>
	struct VecDiv;

	// General case
	template<int32 I, int32 COUNT_I>
	struct VecDiv
	{
		template<class V1, class V2, class V3>
		static void apply(const V1& src1, const V2& src2, V3& dst)
		{
            typedef typename std::remove_reference<decltype(dst[0])>::type T;
			dst[COUNT_I] = T(src1[COUNT_I] / src2[COUNT_I]);
			VecDiv<I, COUNT_I + 1>::apply(src1, src2, dst);
		}
		template<class V1, class V2>
		static void apply(const V1& src1, V2& dst)
		{
			dst[COUNT_I] /= src1[COUNT_I];
			VecDiv<I, COUNT_I + 1>::apply(src1, dst);
		}
	};
	// End
	template<int32 I>
	struct VecDiv<I, I>
	{
		template<class V1, class V2, class V3>
		static void apply(const V1&, const V2&, V3&){}

		template<class V1, class V2>
		static void apply(const V1&, V2&){}
	};

	////////////////////////////////////////////////////
	/// \brief Const value addtion

	// Declaration
	template<int32 I, int32 COUNT_I = 0>
	struct VecAddVal;

	// General case
	template<int32 I, int32 COUNT_I>
	struct VecAddVal
	{
		template<class A1, class A2, typename V>
		static void apply(const A1& src1, A2& dst, const V& value)
		{
            typedef typename std::remove_reference<decltype(dst[0])>::type T;
			dst[COUNT_I] = T(src1[COUNT_I] + value);
			VecAddVal<I, COUNT_I + 1>::apply(src1, dst, value);
		}
		template<class A1, typename V>
		static void apply(A1& src, const V& value)
		{
			src[COUNT_I] += value;
			VecAddVal<I, COUNT_I + 1>::apply(src, value);
		}
	};
	// End
	template<int32 I>
	struct VecAddVal<I, I>
	{
		template<class A1, class A2, typename V>
		static void apply(const A1&, A2&, const V&){}

		template<class A1, typename V>
		static void apply(A1&, const V&){}
	};
	////////////////////////////////////////////////////
	/// \brief Const value substraction

	// Declaration
	template<int32 I, int32 COUNT_I = 0>
	struct VecSubVal;

	// General case
	template<int32 I, int32 COUNT_I>
	struct VecSubVal
	{
		template<class A1, class A2, typename V>
		static void apply(const A1& src1, A2& dst, const V& value)
		{
            typedef typename std::remove_reference<decltype(dst[0])>::type T;
			dst[COUNT_I] = T(src1[COUNT_I] - value);
			VecSubVal<I, COUNT_I + 1>::apply(src1, dst, value);
		}
		template<class A1, typename V>
		static void apply(A1& src, const V& value)
		{
			src[COUNT_I] -= value;
			VecSubVal<I, COUNT_I + 1>::apply(src, value);
		}

	};
	// End
	template<int32 I>
	struct VecSubVal<I, I>
	{
		template<class A1, class A2, typename V>
		static void apply(const A1&, A2&, const V&){}

		template<class A1, typename V>
		static void apply(A1&, const V&){}
	};

	////////////////////////////////////////////////////
	/// \brief Const value multiplication

	// Declaration
	template<int32 I, int32 COUNT_I = 0>
	struct VecMultVal;

	// General case
	template<int32 I, int32 COUNT_I>
	struct VecMultVal
	{
		template<class A1, class A2, typename V>
		static void apply(const A1& src1, A2& dst, const V& value)
		{
            typedef typename std::remove_reference<decltype(dst[0])>::type T;
			dst[COUNT_I] = T(src1[COUNT_I] * value);
			VecMultVal<I, COUNT_I + 1>::apply(src1, dst, value);
		}
		template<class A1, typename V>
		static void apply(A1& src, const V& value)
		{
			src[COUNT_I] *= value;
			VecMultVal<I, COUNT_I + 1>::apply(src, value);
		}
	};
	// End
	template<int32 I>
	struct VecMultVal<I, I>
	{
		template<class A1, class A2, typename V>
		static void apply(const A1&, A2&, const V&){}

		template<class A1, typename V>
		static void apply(A1&, const V&){}
	};
	////////////////////////////////////////////////////
	/// \brief Const value division

	// Declaration
	template<int32 I, int32 COUNT_I = 0>
	struct VecDivVal;

	// General case
	template<int32 I, int32 COUNT_I>
	struct VecDivVal
	{
		template<class A1, class A2, typename V>
		static void apply(const A1& src1, A2& dst, const V& value)
		{
            typedef typename std::remove_reference<decltype(dst[0])>::type T;
			dst[COUNT_I] = T(src1[COUNT_I] / value);
			VecDivVal<I, COUNT_I + 1>::apply(src1, dst, value);
		}
		template<class A1, typename V>
		static void apply(A1& src, const V& value)
		{
			src[COUNT_I] /= value;
			VecDivVal<I, COUNT_I + 1>::apply(src, value);
		}

	};
	// End
	template<int32 I>
	struct VecDivVal<I, I>
	{
		template<class A1, class A2, typename V>
		static void apply(const A1&, A2&, const V&){}

		template<class A1, typename V>
		static void apply(A1&, const V&){}
	};
	////////////////////////////////////////////////////
	/// \brief Copy

	// Declaration
	template<int32 I, int32 COUNT_I = 0>
	struct VecCopy;

	// General case
	template<int32 I, int32 COUNT_I>
	struct VecCopy
	{
		template<class A1, class A2>
		static void apply(const A1& src1, A2& dst)
		{
            typedef typename std::remove_reference<decltype(dst[0])>::type T;
			dst[COUNT_I] = T(src1[COUNT_I]);
			VecCopy<I, COUNT_I + 1>::apply(src1, dst);
		}
	};
	//End
	template<int32 I>
	struct VecCopy<I, I>
	{
		template<class A1, class A2>
		static void apply(const A1&, A2&){}
	};
	////////////////////////////////////////////////////
	/// \brief Scalar product

	// Declaration
	template<int32 I, int32 COUNT_I = 0>
	struct VecScalarProd;

	// First entry
	template<int32 I>
	struct VecScalarProd<I, 0>
	{
		template<class V1, class V2, typename T>
		static void apply(const V1& src1, const V2& src2, T& norm)
		{
			norm = T(src1[0] * src2[0]);
			VecScalarProd<I, 1>::apply(src1, src2, norm);
		}
	};
	// General case
	template<int32 I, int32 COUNT_I>
	struct VecScalarProd
	{
		template<class V1, class V2, typename T>
		static void apply(const V1& src1, const V2& src2, T& norm)
		{
			norm += src1[COUNT_I] * src2[COUNT_I];
			VecScalarProd<I, COUNT_I + 1>::apply(src1, src2, norm);
		}
	};
	//End
	template<int32 I>
	struct VecScalarProd<I, I>
	{
		template<class V1, class V2, typename T>
		static void apply(const V1&, const V2&, T&){}
	};
	////////////////////////////////////////////////////
	/// \brief Norm
	template<int32 I>
	struct VecQuadNorm
	{
		template<class V1, typename T>
		static void apply(const V1& src1, T& norm)
		{
			VecScalarProd<I>::apply(src1, src1, norm);
		}
	};
	////////////////////////////////////////////////////
	/// \brief Matrix product
	/// \note Mat[i,j] = Vec1[i] * Vec2[j]

	// Declaration
	template<int32 I, int32 J,
		int32 COUNT_I = 0, int32 COUNT_J = 0>
	struct VecMatProd;

	// General case
	template<int32 I, int32 J,
		int32 COUNT_I, int32 COUNT_J>
	struct VecMatProd
	{
		template<class V1, class V2, class M>
		static void apply(const V1& src1, const V2& src2, M& dst)
		{
            typedef typename std::remove_reference<decltype(dst[0])>::type T;
			dst[COUNT_I][COUNT_J] = T(src1[COUNT_I] * src2[COUNT_J]);
			VecMatProd<I, J, COUNT_I + 1, COUNT_J>::apply(src1, src2, dst);
		}
	};
	// End of i
	template<int32 I, int32 J, int32 COUNT_J>
	struct VecMatProd<I, J, I, COUNT_J>
	{
		template<class V1, class V2, class M>
		static void apply(const V1& src1, const V2& src2, M& dst)
		{
			VecMatProd<I, J, 0, COUNT_J + 1>::apply(src1, src2, dst);
		}
	};
	// End of j
	template<int32 I, int32 J>
	struct VecMatProd<I, J, 0, J>
	{
		template<class V1, class V2, class M>
		static void apply(const V1& src1, const V2& src2, M& dst){}
	};
	////////////////////////////////////////////////////
	/// \brief Vector product

	// Note: only defined for vectors of size 2 & 3
//	template<int32 I>
	struct VecCross2
	{
		template<class V1, class V2, typename  T>
		static void apply(const V1& src1, const V2& src2, T& dst)
		{
			dst = T(src1[0] * src2[1] - src1[1] * src2[0]);
		}
	};

	struct VecCross3
	{
		template<class V1,class V2,class V3>
		static void apply(const V1& src1, const V2& src2, V3& dst)
		{
            typedef typename std::remove_reference<decltype(dst[0])>::type T;
			dst[0] = T(src1[1] * src2[2]- src1[2] * src2[1]);
			dst[1] = T(src1[2] * src2[0] - src1[0] * src2[2]);
			dst[2] = T(src1[0] * src2[1] - src1[1] * src2[0]);
		}
	};
	struct VecCross
	{
		template<class V1, class V2, class V3>
		static void apply(const V1& src1, const V2& src2, V3& dst)
		{
			VecCross3::apply(src1, src2, dst);
		}
	};
	////////////////////////////////////////////////////
	/// \brief Vector interpolation
	template<int32 I, int32 COUNT_I = 0>
	struct VecInterp;


	template<int32 I, int32 COUNT_I>
	struct VecInterp
	{
		template<class V1,class V2,class V3,typename F>
		static void apply(const V1& src1, const V2& src2, V3& dst,const F& f)
		{
            typedef typename std::remove_reference<decltype(dst[0])>::type T;
            dst[COUNT_I] = T(src1[COUNT_I]*(1-f) + src2[COUNT_I]*f);
            VecInterp<I, COUNT_I + 1>::apply(src1,src2,dst,f);
		}
	};
	template<int32 I>
	struct VecInterp<I,I>
	{
		template<class V1,class V2,class V3,typename F>
		static void apply(const V1& src1, const V2& src2, V3& dst,const F& f)
		{}
	};

	////////////////////////////////////////////////////
	/// \brief Test equality
	template<int32 I, int32 COUNT_I = 0>
	struct VecEqual;

	template<int32 I, int32 COUNT_I>
	struct VecEqual
	{
		template<class V1,class V2>
		static bool apply(const V1& src1, const V2& src2)
		{
            if(src1[COUNT_I] == src2[COUNT_I])
                return VecEqual<I, COUNT_I + 1>::apply(src1,src2);
            else
                return false;
		}
	};

	template<int32 I>
	struct VecEqual<I,I>
	{
		template<class V1,class V2>
		static bool apply(const V1& src1, const V2& src2)
		{
            return true;
		}
	};

	////////////////////////////////////////////////////
	/// \brief Absolute value
	template<int32 I, int32 COUNT_I = 0>
	struct VecAbs;
	template<int32 I, int32 COUNT_I>
	struct VecAbs
	{
		template<class V1, class V2>
		static void apply(const V1& src, V2& dst)
		{
			dst[COUNT_I] = util::Abs(src[COUNT_I]);
			VecAbs<I,COUNT_I + 1>::apply(src, dst);
		}
		template<class V1>
		static void apply(V1& src)
		{
			src[COUNT_I] = util::Abs(src[COUNT_I]);
			VecAbs<I, COUNT_I + 1>::apply(src);
		}
	};

	template<int32 I>
	struct VecAbs<I, I>
	{
		template<class V1, class V2>
		static void apply(const V1& src,V2& dst)
		{}
		template<class V1>
		static void apply(V1& src)
		{}
	};
	////////////////////////////////////////////////////
	/// \brief Sign
	template<int32 I, int32 COUNT_I = 0>
	struct VecSign;
	template<int32 I, int32 COUNT_I>
	struct VecSign
	{
		template<class V1, class V2>
		static void apply(const V1& src, V2& dst)
		{
			typedef typename std::remove_reference<decltype(dst[0])>::type T;
			dst[COUNT_I] = src[COUNT_I] >= 0 ? T(1):T(-1);
			VecSign<I, COUNT_I + 1>::apply(src, dst);
		}
		template<class V1>
		static void apply(V1& src)
		{
			typedef typename std::remove_reference<decltype(src[0])>::type T;
			src[COUNT_I] = src[COUNT_I] >= 0 ? T(1):T(-1);
			VecSign<I, COUNT_I + 1>::apply(src);
		}
	};

	template<int32 I>
	struct VecSign<I, I>
	{
		template<class V1, class V2>
		static void apply(const V1& src, V2& dst)
		{}
		template<class V1>
		static void apply(V1& src)
		{}
	};
} //core
#endif
