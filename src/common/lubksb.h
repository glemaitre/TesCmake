/// \file lubksb.h
/// \brief LU factorisation
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date September 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef NRC_LUBKSB_H
#define NRC_LUBKSB_H

#include "typedef.h"

namespace nrc
{
template<class A,class Indx,class B>
void Lubksb(const A& a,int32  n,Indx& indx,B& b)
{
	int32  i,ii=-1,ip,j;
	float_t sum;

	for (i=0;i<n;i++) {
		ip=indx[i];
		sum=b[ip];
		b[ip]=b[i];
		if (ii >= 0)
			for (j=ii;j<=i-1;j++) sum -= a[i][j]*b[j];
		else if (sum) ii=i;
		b[i]=sum;
	}
	for (i=n-1;i>=0;i--) {
		sum=b[i];
		for (j=i+1;j<n;j++) sum -= a[i][j]*b[j];
		b[i]=sum/a[i][i];
	}
}
} // nrc
#endif
