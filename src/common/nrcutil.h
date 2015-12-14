/// \file nrcutil.h
/// \brief Utility for nrc
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date September 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef NRC_UTIL_H
#define NRC_UTIL_H

#include "typedef.h"
#include "math.h"
namespace nrc
{

const float_t Eps = 2.e-6;
const float_t Epss = 6.e-8;
const float_t Tiny = 1e-20;

const int32 MaxIt = 100;
const int32 Maxm = 100;
const int32 Radix = 2;
// New
template<typename T>
inline T** NrcNew(int32  m,int32  n)
{
	T** a = new T*[m];
	a[0] = new T[m*n];
	for(int32  i = 1;i < m;++i)
		a[i] = a[i-1] + n;
	return a;
}
template<typename T>
inline void NrcDelete(T** a)
{
	delete[] a[0];
	delete a;
	a = 0;
}

//Copy
template<class Arrs,class Arrd>
inline void NrcCopy(const Arrs& src,int32  nx,int32  ny,Arrd& dst)
{
	//Boucle sur les pts
	for(int32  i = 0;i < nx;++i)
		for(int32  j = 0; j <ny;++j)
			dst[i][j] = src[i][j];
}
//Swap
template<typename T>
void NrcSwap(T& a,T& b)
{
	T tmp = a;
	a = b;
	b = tmp;
}
//Min et Max
template<typename T,typename U>
T NrcMax(const T& a,const U& b)
{return a > b ? a : b;}
template<typename T,typename U>
T NrcMin(const T& a,const U& b)
{return a < b ? a : b;}
//Abs
template<typename T>
T NrcAbs(const T& a)
{return a > 0 ? a : -a;}
//Sign
template<typename T,typename U>
T NrcSign(const T& a,const U& b)
{return b >= 0 ? NrcAbs(a): -NrcAbs(a);}
template<typename T>
int32  NrcSign(const T& b)
{return b >= 0 ? 1:-1;}

template<typename T>
T NrcSqr(const T& a)
{
	return a*a;
}

//Pytag
template<typename T,typename U>
T NrcPythag(const T& a,const U& b)
{
	return sqrt(NrcSqr(a) + NrcSqr(b));
}


template<typename T>
void ShiftVecLeft(T** vec)
{
	--*vec;
}

template<typename T>
void ShiftVecRight(T** vec)
{
	++*vec;
}
template<typename T>
inline void ShiftMatLeft(T*** mat,int32  m)
{
	int32  i;
	T** mat_ = *mat;
	for(i = 0;i < m;i++)
		mat_[i]--;
	mat_--;
	*mat = mat_;
}
template<typename T>
inline void ShiftMatRight(T*** mat,int32  m)
{
	int32  i;
	T** mat_ = *mat;
	mat_++;
	for(i = 0;i < m;i++)
		mat_[i]++;
	*mat = mat_;
}
/////////////////////////////////////////////////////
//
template<class V>
void Sort(V& v)
{
	Sort(&v[0],v.size());
}
template<typename T>
void Sort(T* ra,int32  n)
{
	int32  l,j,ir,i;
	T rra;
	l=(n >> 1);
	ir=n-1;
	for (;;)
	{
		if (l > 0)
			rra=ra[--l];
		else
		{
			rra=ra[ir];
			ra[ir]=ra[0];
			if (--ir == 0)
			{
				ra[0]=rra;
				return;
			}
		}
		i=l;
		j=l << 1;
		while (j <= ir)
		{
			if (j < ir && ra[j] < ra[j+1]) ++j;
			if (rra < ra[j])
			{
				ra[i]=ra[j];
				j += (i=j)+1;
			}
			else j=ir+1;
		}
		ra[i]=rra;
	}
}
} // nrc
#endif
