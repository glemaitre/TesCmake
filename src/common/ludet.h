/// \file invert.h
/// \brief LU factorisation
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date September 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef NRC_LUDET_H
#define NRC_LUDET_H

#include <math.h>
#include "typedef.h"
#include "nrcutil.h"
#include "ludcmp.h"
#include "lubksb.h"

namespace nrc
{

template<class A>
inline float_t LuDet(A& a,int32  n)
{
	float_t d,result = 0;
	int32 * indx = new int32 [n];
	Ludcmp(a,n,indx,d);
	for(int32  i = 0;i < n;++i)
		result += a[i][i];
	delete[] indx;
	return result;
}

//Determinant Lu
template<class A>
inline float_t LuDet(const A& a)
{
	int32  n = a.size_x();
	float_t** a_ = NrcNew<float_t>(n,n);
	NrcCopy(a,n,n,a_);
	return LuDet(a_,n);
	NrcDelete(a_);
}
} // nrc
#endif
