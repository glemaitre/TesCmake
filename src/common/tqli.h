/// \file tqli.h
/// \brief Eigen system
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date September 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef NRC_TQLI_H
#define NRC_TQLI_H

#include "typedef.h"
#include "nrcutil.h"

namespace nrc
{
template<class D,class E,class Z>
void Tqli(D& d,E& e,int32  n,Z& z)
{
	int32  m,l,iter,i,k;
	float_t s,r,p,g,f,dd,c,b;

	for (i=1;i<n;i++) e[i-1]=e[i];
	e[n-1]=0;
	for (l=0;l<n;l++) {
		iter=0;
		do {
			for (m=l;m < n-1;m++) {
				dd=NrcAbs(d[m])+NrcAbs(d[m+1]);
				if ((NrcAbs(e[m])+dd) == dd) break;
			}
			if (m != l) {
				if (iter++ == 30) throw "Too many iterations in TQLI";
				g=(d[l+1]-d[l])/(2*e[l]);
				r=sqrt((g*g)+1);
				g=d[m]-d[l]+e[l]/(g+NrcSign(r,g));
				s=c=1;
				p=0;
				for (i=m-1;i>=l;i--) {
					f=s*e[i];
					b=c*e[i];
					if (NrcAbs(f) >= NrcAbs(g)) {
						c=g/f;
						r=sqrt((c*c)+1);
						e[i+1]=f*r;
						c *= (s=1/r);
					} else {
						s=f/g;
						r=sqrt((s*s)+1);
						e[i+1]=g*r;
						s *= (c=1/r);
					}
					g=d[i+1]-p;
					r=(d[i]-g)*s+2*c*b;
					p=s*r;
					d[i+1]=g+p;
					g=c*r-b;
					/* Next loop can be omitted if eigenvectors not wanted */
					for (k=0;k<n;k++) {
						f=z[k][i+1];
						z[k][i+1]=s*z[k][i]+c*f;
						z[k][i]=c*z[k][i]-s*f;
					}
				}
				d[l]=d[l]-p;
				e[l]=g;
				e[m]=0;
			}
		} while (m != l);
	}
}
}
#endif
