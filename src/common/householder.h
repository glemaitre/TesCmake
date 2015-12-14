/// \file householder.h
/// \brief Eigen system
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date September 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr


#ifndef NRC_HOUSEHOLDER_H
#define NRC_HOUSEHOLDER_H

#include "typedef.h"
#include "nrcutil.h"
#include "tqli.h"
#include "tred2.h"

namespace nrc
{
template<class A,class V>
inline void HouseHolder(A& a,int32  n,V& v)
{
	float_t* e = new float_t[n];
	Tred2(a,n,v,e);
	Tqli(v,e,n,a);
	delete[] e;
}
template<class A,class V,class Q>
inline void HouseHolder(const A& a,int32  n,V& v,Q& q)
{
	NrcCopy(a,n,n,q);
	HouseHolder(q,n,v);
}
template<class A,class V,class Q>
inline void HouseHolder(const A& a,V& v,Q& q)
{
	HouseHolder(a,a.size_x(),v,q);
}
template<class A,class V>
inline void HouseHolder(A& a,V& v)
{
	HouseHolder(a,a.size_x(),v);
}
} // nrc
#endif
