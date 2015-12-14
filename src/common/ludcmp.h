/// \file ludcmp.h
/// \brief LU factorisation
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date September 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef NRC_LUDCMP_H
#define NRC_LUDCMP_H

#include "typedef.h"
#include "nrcutil.h"

namespace nrc
{
template<class A,class Indx,typename T>
void Ludcmp(A& a,int32  n,Indx& indx,T& d)
{
	int32  i,imax,j,k;
	float_t big,dum,sum,temp;
	float_t* vv = new float_t[n];

	d=1;
	for (i=0;i < n;i++) {
		big=0;
		for (j = 0;j<n;j++)
			if ((temp = NrcAbs(a[i][j])) > big) big=temp;
		if (big == 0) throw "Singular matrix in routine LUDCMP";
		vv[i]=1/big;
	}
	for (j= 0;j < n;j++) {
		for (i=0;i<j;i++) {
			sum=a[i][j];
			for (k=0;k<i;k++) sum -= a[i][k]*a[k][j];
			a[i][j]=sum;
		}
		big=0;
		for (i=j;i<n;i++) {
			sum=a[i][j];
			for (k=0;k<j;k++)
				sum -= a[i][k]*a[k][j];
			a[i][j]=sum;
			if ( (dum=vv[i]*NrcAbs(sum)) >= big) {
				big=dum;
				imax=i;
			}
		}
		if (j != imax) {
			for (k=0;k<n;k++) {
				dum=a[imax][k];
				a[imax][k]=a[j][k];
				a[j][k]=dum;
			}
			d = -d;
			vv[imax]=vv[j];
		}
		indx[j]=imax;
		if (a[j][j] == 0) a[j][j]=Tiny;
		if (j != n) {
			dum=1/(a[j][j]);
			for (i=j+1;i<n;i++) a[i][j] *= dum;
		}
	}
	delete[] vv;
}
}
#endif
