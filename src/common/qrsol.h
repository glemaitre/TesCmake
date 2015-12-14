/// \file qrsol.h
/// \brief QR factorisation
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date September 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef NRC_QRSOL_H
#define NRC_QRSOL_H

#include <math.h>
#include "typedef.h"
#include "nrcutil.h"
#include "qrdcmp.h"
#include "qrsolv.h"

namespace nrc
{
//Résolution Qr
template<class A,class B,class X>
inline void QrSol(const A& a_in,int32  n,const B& b,X& x)
{
	int32  sing;
	float_t** a = NrcNew<float_t>(n,n);
	float_t* c = new float_t[n];
	float_t* d = new float_t[n];
	NrcCopy(a_in,n,n,a);
	for(int32  i = 0;i < n;i++)
			x[i] = b[i];
	Qrdcmp(a,n,c,d,sing);
	Qrsolv(a,n,c,d,x);
	delete[] c;
	delete[] d;
	NrcDelete(a);
}
template<class A,class B>
inline void QrSol(A& a,int32  n,B& b)
{
	int32  sing;
	float_t* c = new float_t[n];
	float_t* d = new float_t[n];
	Qrdcmp(a,n,c,d,sing);
	Qrsolv(a,n,c,d,b);
	delete[] c;
	delete[] d;
}
template<class A,class B>
inline void QrSol(A& a,B& b)
{
	QrSol(a,a.size_x(),b);
}
template<class A,class B,class X>
inline void QrSol(const A& a,const B& b,X& x)
{
	QrSol(a,a.size_x(),b,x);
}
} // nrc
#endif
