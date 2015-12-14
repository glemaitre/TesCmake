/// \file unpackq.h
/// \brief Eigen system
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date September 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef NRC_UNPACKQ_H_
#define NRC_UNPACKQ_H

#include "typedef.h"

namespace nrc
{
template<class A,class T,class Q>
void Unpackq(const A& a,int32  n,const T& tau,Q& q)
{
	//Init
	int32 i,j,k;
	float_t v;
	for(i = 0;i < n;++i)
		for(j = 0;j < n;++j)
			q[i][j] = 0;
	for(i = 0;i < n;++i)
		q[i][i] = 1;
	//unpack Q
	for(i = 0;i <(n-1);++i)
	{
		if(tau[i] != 0)
		{
			for(j = 0;j<n;++j)
			{
				v = q[j][i+1];
				for(k = i+2; k < n;k++)
					v+= q[j][k]*a[k][i];
				v *= tau[i];
				q[j][i+1]-=v;
				for(k = i+2; k < n;k++)
					q[j][k]-= a[k][i]*v;
			}
		}
	}
}
} // nrc
#endif
