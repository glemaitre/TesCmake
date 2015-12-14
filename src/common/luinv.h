/// \file luinv.h
/// \brief LU factorisation
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date September 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef NRC_LUINV_H
#define NRC_LUINV_H

#include <math.h>
#include "typedef.h"
#include "nrcutil.h"
#include "ludcmp.h"
#include "lubksb.h"

namespace nrc
{

template<class A>
void LuInv(A& a,int32  n)
{
	float_t d;
	int32 * indx = new int32 [n];
	float_t** a_ = NrcNew<float_t>(n,n);
	float_t* col = new float_t[n];
	NrcCopy(a,n,n,a_);
	Ludcmp(a_,n,indx,d);
	for(int32  j = 0;j < n;j++)
	{
		for(int32  i = 0;i < n;i++)
			col[i] = 0;
		col[j] = 1;
		Lubksb(a_,n,indx,col);
		for(int32  i = 0;i < n;i++)
			a[i][j] = col[i];
	}
	delete[] col;
	delete[] indx;
	NrcDelete(a_);

}

//Inversion Lu
template<class AIn,class AOut>
inline void LuInv(const AIn& a_in,AOut& a_out)
{
	int32  n = a_in.size_x();
	NrcCopy(a_in,n,n,a_out);
	LuInv(a_out,n);
}
template<class A>
inline void LuInv(A& a)
{
	LuInv(a,a.size_x());
}
template<class AIn,class AOut>
inline void LuInv(const AIn& a_in,int32  n,AOut& a_out)
{
	NrcCopy(a_in,n,n,a_out);
	LuInv(a_out,n);
}
}
#endif
