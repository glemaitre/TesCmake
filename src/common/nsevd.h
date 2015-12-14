/// \file nsevd.h
/// \brief Eigen system
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date September 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef NRC_NSEVD_H
#define NRC_NSEVD_H

#include "typedef.h"
#include "nrcutil.h"
#include "elmhes.h"
#include "hseqr.h"
#include "trevc.h"
#include "unpackq.h"

namespace nrc
{
template<class A,class W>
inline void NsEvd(A& a,int32  n,W& wr,W& wi)
{
	Elmhes(a,n);
	Hseqr(a,n,wr,wi);
}

template<class A,class W,class V>
inline void NsEvd(A& a,int32  n,W& wr,W& wi,V& v)
{
	float_t** t = 0;
	Elmhes(a,n,wr);
	Unpackq(a,n,wr,v);
	Hseqr(a,n,wr,wi,v);
	Trevc(a,n,1,t,v);
}

template<class A,class W,class VL,class VR>
inline void NsEvd(A& a,int32  n,W& wr,W& wi,VL& vl,VR& vr)
{
    float_t** t = 0;
	Elmhes(a,n,wr);
	Unpackq(a,n,wr,vl);
	Hseqr(a,n,wr,wi,vl);
	Trevc(a,n,3,t,vl,vr);
}
template<class A,class W,class V>
inline void NsEvd(A& a,int32  n,int32  side,W& wr,W& wi,V& v)
{
	float_t** t = 0;
	Elmhes(a,n,wr);
	Unpackq(a,n,wr,v);
	Hseqr(a,n,wr,wi,v);
	if(side & 1)
		Trevc(a,n,1,t,v);
	else
		Trevc(a,n,2,v,t);
}
template<class A,class W>
inline void NsEvd(A& a,W& wr,W& wi)
{
	NsEvd(a,a.size_x(),wr,wi);
}
} // nrc
#endif
