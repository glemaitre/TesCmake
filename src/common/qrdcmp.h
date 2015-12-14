/// \file qrdcmp.h
/// \brief QR factorisation
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date September 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr
#ifndef NRC_QRDCMP_H
#define NRC_QRDCMP_H

#include "typedef.h"
#include "nrcutil.h"

namespace nrc
{

template<class A,class C,class D>
void Qrdcmp(A& a,int32  n,C& c,D& d,int32 & sing)
{
	int32  i,j,k;
	float_t scale,norm,sigma,sum,tau;
	sing = 0;

	for(k=0; k <n-1;k++)
	{
		scale = 0;
		for(i = k;i < n;i++)
			scale = NrcMax(scale,fabs(a[i][k]));
		if(scale == 0)
		{
			sing = 1;
			c[k] = d[k] = 0;
		}
		else
		{
			norm = 1/scale;
			for(sum = 0,i= k;i < n;i++)
			{
				a[i][k] *= norm;
				sum += NrcSqr(a[i][k]);
			}
			sigma = NrcSign(sqrt(sum),a[k][k]);
			a[k][k] += sigma;
			c[k] = sigma * a[k][k];
			d[k] = -scale*sigma;
			for(j=k+1;j < n;j++)
			{
				for(sum = 0,i= k;i < n;i++)
					sum += a[i][k] * a[i][j];
				tau = sum/c[k];
				for(i = k;i < n;i++)
					a[i][j] -= tau*a[i][k];
			}
		}
	}
	k = n-1;
	d[k]=a[k][k];
	if(d[k]==0)
		sing = 1;
}

template<class A,class C,class D,class Q>
void Qrunpack(A& a,int32  n,C& c,D& d,Q& q)
{
	int32  i,j,k;
	float_t tau,v;
	//Initialisation de q
	tau = 1/c[0];
	for(i = 0;i < n;++i)
	{
		for(j = i;j < n;++j)
			q[i][j] = -a[i][0] * a[j][0] * tau;
		q[i][i] += 1;
		for(j = i+1;j < n;++j)
			q[j][i] = q[i][j];
	}
	//Produit des n-1 matrices de Householder
	for(i = 1;i < n-1;++i)
	{
		tau = 1/c[i];
		for(j = 0;j < n;++j)
		{
			v = 0;
			for(k = i;k < n;++k)
				v += q[j][k]*a[k][i];
			v*=tau;
			for(k = i;k < n;++k)
				q[j][k] -= v * a[k][i];
		}
	}

	//Construction du R
	for(int32  i = 0;i < n;i++)
		for(int32  j = 0;j < i;++j)
			a[i][j] = 0;
	for(int32  i = 0;i < n;i++)
		a[i][i] = d[i];
}
}

#endif
