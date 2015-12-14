/// \file elmhes.h
/// \brief Eigen system
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date September 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef NRC_ELMHES_H
#define NRC_ELMHES_H

#include "typedef.h"

namespace nrc
{

template<class A>
void Elmhes(A& a,int32  n)
{
	int32  m,j,i;
	float_t y,x;
	for (m=1;m < (n-1);m++) {
		x=0.0;
		i=m;
		for (j=m;j<n;j++) {
			if (NrcAbs(a[j][m-1]) > NrcAbs(x)) {
				x=a[j][m-1];
				i=j;
			}
		}
		if (i != m) {
			for (j=m-1;j<n;j++) NrcSwap(a[i][j],a[m][j]);
			for (j=0;j<n;j++) NrcSwap(a[j][i],a[j][m]);
		}
		if (x) {
			for (i=m+1;i<n;i++) {
				if ((y=a[i][m-1])) {
					y /= x;
					a[i][m-1]=y;
					for (j=m;j<n;j++)
						a[i][j] -= y*a[m][j];
					for (j=0;j<n;j++)
						a[j][m] += y*a[j][i];
				}
			}
		}
	}
}

template<class A,class T>
void Elmhes(A& a,int32  n,T& tau)
{
	int32  i,j,k;
	float_t scale,h,w,anorm;
	float_t* v = new float_t[n];
	anorm = 0;
	for(i = 0;i < n;++i)
		for(j = 0;j < n;++j)
			if(NrcAbs(a[i][j])> anorm)
				anorm = NrcAbs(a[i][j]);
	anorm = 1/anorm;
	for(i = 0;i < (n-1);++i)
	{
		//Generate reflexion
		scale=0;
		for(j = i+2;j < n;++j)
			scale+= NrcSqr(a[j][i]*anorm);
		if(scale)
		{
			scale += NrcSqr(a[i+1][i] * anorm);
			scale = sqrt(scale)/anorm;
			h = a[i+1][i];
			if(h >= 0)
				scale = -scale;
			a[i+1][i] = scale;

			tau[i] = 1-h/scale;
			scale = 1/(h-scale);
			v[i+1] = 1;
			for(j = i+2;j < n;++j)
				v[j] = a[j][i] *= scale;


			for(j = 0;j<n;++j)
			{
				w = 0;
				for(k = i+1; k < n;k++)
					w+= a[j][k]*v[k];
				w *= tau[i];
				for(k = i+1; k < n;k++)
					a[j][k] -= v[k]*w;
			}

			for(j = i+1;j<n;++j)
			{
				w = 0;
				for(k = i+1; k < n;k++)
					w+= a[k][j]*v[k];
				w *= tau[i];
				for(k = i+1; k < n;k++)
					a[k][j] -= v[k]*w;
			}
		}
		else
			tau[i] = 0;
	}
	delete[] v;
}
}
#endif
