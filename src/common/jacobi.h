/// \file jacobi.h
/// \brief Eigen system
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date September 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef NRC_JACOBI_H
#define NRC_JACOBI_H

#include "typedef.h"
#include "nrcutil.h"

namespace nrc
{

template<typename T>
inline void Rotate(T& x,T& y,const T s,const T tau)
{
    T g = x;
    T h = y;
    x -= s*(h+g*tau);
    y += s*(g-h*tau);
}

//Valeurs propre
template<class A,class D,class V>
void Jacobi(A& a,int32  n,D& d,V& v,int32 & nrot)
{
	int32  j,iq,ip,i;
	float_t tresh,theta,tau,t,sm,s,h,g,c,*b,*z;

	b = new float_t[n];
	z = new float_t[n];

	for (ip=0;ip<n;ip++) {
		for (iq=0;iq<n;iq++) v[ip][iq]=0.0;
		v[ip][ip]=1.0;
	}
	for (ip=0;ip<n;ip++) {
		b[ip]=d[ip]=a[ip][ip];
		z[ip]=0.0;
	}
	nrot=0;
	for (i=1;i<=50;i++) {
		sm=0.0;
		for (ip=0;ip<n-1;ip++) {
			for (iq=ip+1;iq<n;iq++)
				sm += NrcAbs(a[ip][iq]);
		}
		if (sm == 0.0) {
			delete[] z;
			delete[] b;
			return;
		}
		if (i < 4)
			tresh=.2*sm/(n*n);
		else
			tresh=0;
		for (ip=0;ip< n-1;ip++) {
			for (iq=ip+1;iq<n;iq++) {
				g=100*NrcAbs(a[ip][iq]);
				if (i > 4 && (NrcAbs(d[ip])+g) == NrcAbs(d[ip])
					&& (NrcAbs(d[iq])+g) == NrcAbs(d[iq]))
					a[ip][iq]=0.0;
				else if (NrcAbs(a[ip][iq]) > tresh) {
					h=d[iq]-d[ip];
					if ((NrcAbs(h)+g) == NrcAbs(h))
						t=(a[ip][iq])/h;
					else {
						theta=(0.5)*h/(a[ip][iq]);
						t=(1/(NrcAbs(theta)+sqrt(1+theta*theta)));
						if (theta < 0.0) t = -t;
					}
					c=1/sqrt(1+t*t);
					s=t*c;
					tau=s/(1+c);
					h=t*a[ip][iq];
					z[ip] -= h;
					z[iq] += h;
					d[ip] -= h;
					d[iq] += h;
					a[ip][iq]=0.0;
					for (j=0;j<=ip-1;j++)
					{
                        Rotate(a[j][ip],a[j][iq],s,tau);
						//ROTATE(a,j,ip,j,iq)
					}
					for (j=ip+1;j<=iq-1;j++)
					{
                        Rotate(a[ip][j],a[j][iq],s,tau);
						// ROTATE(a,ip,j,j,iq)
					}
					for (j=iq+1;j<n;j++)
					{
                        Rotate(a[ip][j],a[iq][j],s,tau);
						//ROTATE(a,ip,j,iq,j)
					}
					for (j=0;j<n;j++)
					{
                        Rotate(v[j][ip],v[j][iq],s,tau);
						//ROTATE(v,j,ip,j,iq)
					}
					++(nrot);
				}
			}
		}
		for (ip=0;ip<n;ip++) {
			b[ip] += z[ip];
			d[ip]=b[ip];
			z[ip]=0.0;
		}
	}
	delete[] z;
	delete[] b;
	throw "Too many iterations in routine JACOBI";
}
template<class A,class Aout,class V,class Q>
inline void Jacobi(const A& a,Aout& a_out,V& v,Q& q)
{
	int32  nrot;
	Jacobi(a,a.size_x(),a_out,v,q,nrot);
}
template<class Arr,class Aout,class V,class Q>
inline void Jacobi(Arr& a,int32  n,Aout& a_out,V& v,Q& q)
{
	int32  nrot;
	NrcCopy(a,n,n,a_out);
	Jacobi(a_out,n,v,q,nrot);
}
//Valeurs propre
template<class Arr,class V,class Q>
inline void Jacobi(Arr& a,V& v,Q& q)
{
	int32  nrot;
	Jacobi(a,a.size_x(),v,q,nrot);
}
template<class Arr,class V,class Q>
inline void Jacobi(Arr& a,int32  n,V& v,Q& q)
{
	int32  nrot;
	Jacobi(a,n,v,q,nrot);
}
#undef ROTATE
} // nrc
#endif
