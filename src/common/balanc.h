/// \file balanc.h
/// \brief balancing a matrix
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date September 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef NRC_BALANC_H
#define NRC_BALANC_H

#include "nrcutil.h"
#include "typedef.h"

namespace nrc
{

template<class A>
void Balanc(A& a,int32  n)
{
	int32  last,j,i;
	float_t s,r,g,f,c,sqrdx;

	sqrdx = Radix * Radix ;
	last=0;
	while (last == 0) {
		last=1;
		for (i=0;i< n;i++) {
			r=c=0.0;
			for (j=0;j< n;j++)
				if (j != i) {
					c += NrcAbs(a[j][i]);
					r += NrcAbs(a[i][j]);
				}
			if (c && r) {
				g=r/Radix;
				f=1.0;
				s=c+r;
				while (c<g) {
					f *= Radix;
					c *= sqrdx;
				}
				g = r * Radix;
				while (c>g) {
					f /= Radix;
					c /= sqrdx;
				}
				if ((c+r)/f < 0.95*s) {
					last=0;
					g=1.0/f;
					for (j=0;j< n;j++) a[i][j] *= g;
					for (j=0;j< n;j++) a[j][i] *= f;
				}
			}
		}
	}
}
} // nrc
#endif
