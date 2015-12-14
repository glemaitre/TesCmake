/// \file lusol.h
/// \brief LU factorisation
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date September 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef NRC_LUSOL_H
#define NRC_LUSOL_H

#include <math.h>
#include "typedef.h"
#include "nrcutil.h"
#include "ludcmp.h"
#include "lubksb.h"

namespace nrc
{
//Résolution Lu
template<class A,class B,class X>
inline void LuSol(const A& a_in,int32  n,const B& b,X& x)
{
	float_t d;
	float_t** a = NrcNew<float_t>(n,n);
	int32 * indx = new int32 [n];
	NrcCopy(a_in,n,n,a);
	for(int32  i = 0;i < n;i++)
			x[i] = b[i];
	Ludcmp(a,n,indx,d);
	Lubksb(a,n,indx,x);
	delete[] indx;
	NrcDelete(a);
}

template<class A,class B,class X>
void LuSolN(const A& a_in,int32  n,const B& b,X& x,int32  m)
{
	float_t d;
	float_t** a = NrcNew<float_t>(n,n);
	NrcCopy(a_in,n,n,a);

	int32 * indx = new int32 [n];
	float_t* col = new float_t[n];
	Ludcmp(a,n,indx,d);

	for(int32  i = 0;i < m;i++)
	{
		for(int32  j = 0;j < n;j++)
			col[j] = b[j][i];
		Lubksb(a,n,indx,col);
		for(int32  j = 0;j < n;j++)
			x[j][i] = col[j];
	}
	delete[] col;
	delete[] indx;
	NrcDelete(a);
}
template<class A,class B>
inline void LuSol(A& a,int32  n,B& b)
{
	float_t d;
	int32 * indx = new int32 [n];
	Ludcmp(a,n,indx,d);
	Lubksb(a,n,indx,b);
	delete[] indx;
}
template<class A,class B>
void LuSolN(A& a,int32  n,B& b,int32  m)
{
	float_t d;
	int32 * indx = new int32 [n];
	float_t* col = new float_t[n];
	Ludcmp(a,n,indx,d);

	for(int32  i = 0;i < m;i++)
	{
		for(int32  j = 0;j < n;j++)
			col[j] = b[j][i];
		Lubksb(a,n,indx,col);
		for(int32  j = 0;j < n;j++)
			b[j][i] = col[j];
	}
	delete[] col;
	delete[] indx;
}
template<class A,class B>
inline void LuSol(A& a,B& b)
{
	LuSol(a,a.size_x(),b);
}
template<class A,class B>
inline void LuSolN(A& a,B& b)
{
	LuSolN(a,a.size_x(),b,b.size_y());
}
template<class A,class B,class X>
inline void LuSol(const A& a,const B& b,X& x)
{
	LuSol(a,a.size_x(),b,x);
}
template<class A,class B,class X>
inline void LuSolN(const A& a,const B& b,X& x)
{
	LuSolN(a,a.size_x(),b,x,b.size_y());
}
} // nrc
#endif
