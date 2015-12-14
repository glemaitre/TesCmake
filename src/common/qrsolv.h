/// \file qrsolv.h
/// \brief QR factorisation
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date September 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef NRC_QRSOLV_H
#define NRC_QRSOLV_H

#include "typedef.h"
#include "nrcutil.h"

namespace nrc
{

template<class A,class C,class D,class B>
void Qrsolv(A& a,int32  n,C& c,D& d,B& b)
{
	int32  i,j;
	float_t sum,tau;
	for(j=0;j < (n-1);++j)
	{
		for(sum = 0,i = j; i < n;i++)
			sum += a[i][j]*b[i];
		tau = sum/c[j];
		for(i = j;i < n;i++)
			b[i] -= tau * a[i][j];
	}
	b[n-1] /= d[n-1];
	for(i= n-2;i >=0;i--)
	{
		for(sum = 0,j = i+1; j < n;j++)
			sum+= a[i][j]*b[j];
		b[i]= (b[i]-sum)/d[i];
	}
}
}

#endif
