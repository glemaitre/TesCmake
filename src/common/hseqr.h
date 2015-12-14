/// \file hseqr.h
/// \brief Eigen system
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date September 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef NRC_HSEQR_H
#define NRC_HSEQR_H

#include "typedef.h"
#include "nrcutil.h"
#include <limits>

namespace nrc
{

//espace anonyme
namespace
{
void eig2x2(float_t& a,
     float_t& b,
     float_t& c,
     float_t& d,
     float_t& rt1r,
     float_t& rt1i,
     float_t& rt2r,
     float_t& rt2i,
     float_t& cs,
     float_t& sn)
{
    float_t multpl,aa,bb,bcmax,bcmis,cc,cs1;
    float_t dd,eps,p,sab,sac,scl,sigma;
    float_t sn1,tau,temp,z;

    multpl = 4;
    eps = std::numeric_limits<float_t>::epsilon();
    if( c==0 )
    {
        cs = 1;
        sn = 0;
    }
    else
    {
        if( b==0 )
        {
            cs = 0;
            sn = 1;
            temp = d;
            d = a;
            a = temp;
            b = -c;
            c = 0;
        }
        else
        {
            if((a-d)==0 && NrcSign(b)!= NrcSign(c) )
            {
                cs = 1;
                sn = 0;
            }
            else
            {
                temp = a-d;
                p = 0.5*temp;
                bcmax = NrcMax(NrcAbs(b), NrcAbs(c));
                bcmis = NrcMin(NrcAbs(b), NrcAbs(c))*NrcSign(b)*NrcSign(c);
                scl = NrcMax(NrcAbs(p), bcmax);
                z = p/scl*p+bcmax/scl*bcmis;

                 if( z>=multpl*eps )
                {
                    z = p+NrcSign(sqrt(scl)*sqrt(z),p);
                    a = d+z;
                    d = d-bcmax/z*bcmis;

                    tau = NrcPythag(c, z);
                    cs = z/tau;
                    sn = c/tau;
                    b = b-c;
                    c = 0;
                }
                else
                {
                    sigma = b+c;
                    tau = NrcPythag(sigma, temp);
                    cs = sqrt(0.5*(1+NrcAbs(sigma)/tau));
                    sn = -p/(tau*cs)*NrcSign((float_t)1,sigma);

                    aa = a*cs+b*sn;
                    bb = -a*sn+b*cs;
                    cc = c*cs+d*sn;
                    dd = -c*sn+d*cs;

                    a = aa*cs+cc*sn;
                    b = bb*cs+dd*sn;
                    c = -aa*sn+cc*cs;
                    d = -bb*sn+dd*cs;
                    temp = 0.5*(a+d);
                    a = temp;
                    d = temp;
                    if( c!=0 )
                    {
                        if( b!=0 )
                        {
                            if( NrcSign(b)==NrcSign(c) )
                            {
                                sab = sqrt(fabs(b));
                                sac = sqrt(fabs(c));
                                p = NrcSign(sab*sac, c);
                                tau = 1/sqrt(fabs(b+c));
                                a = temp+p;
                                d = temp-p;
                                b = b-c;
                                c = 0;
                                cs1 = sab*tau;
                                sn1 = sac*tau;
                                temp = cs*cs1-sn*sn1;
                                sn = cs*sn1+sn*cs1;
                                cs = temp;
                            }
                        }
                        else
                        {
                            b = -c;
                            c = 0;
                            temp = cs;
                            cs = -sn;
                            sn = temp;
                        }
                    }
                }
            }
        }
    }
    rt1r = a;
    rt2r = d;
    if( c==0 )
    {
        rt1i = 0;
        rt2i = 0;
    }
    else
    {
        rt1i = sqrt(NrcAbs(b))*sqrt(fabs(c));
        rt2i = -rt1i;
    }
}
void reflec(float_t* x, int32  n, float_t& tau)
{
    int32  j;
	float_t alpha,v,beta;
    tau = 0;
    alpha = x[0];
	beta = 0;
	for(j = 0; j < n; j++)
		beta += NrcSqr(x[j]);
	beta = sqrt(beta);
	if(alpha >= 0)
		beta = -beta;
	if(beta == alpha)
		return;
	tau = (beta-alpha)/beta;
    v = 1/(alpha-beta);
	for(j = 1; j < n; j++)
		x[j] *= v;
    x[0] = beta;
}

template<class H>
float_t hsnorm(const H& h,int32  i1,int32  i2)
{
	int32  i,j;
	float_t norm,res;
	res = 0;
	for(j = i1;j < i2;++j)
	{
		norm = 0;
		for(i = NrcMin((j+1),i2-1); i >= i1;i--)
			norm += NrcAbs(h[i][j]);
		if(norm > res)
			res = norm;
	}
	return res;
}
}
template<class A,class W>
void Hseqr(A& a,int32  n,W& wr,W& wi)
{
	int32  nn,m,l,k,j,its,i,mmin;
	float_t z,y,x,w,v,u,t,s,r,q,p,anorm;

	anorm = NrcAbs(a[0][0]);
	for (i=1;i < n;i++)
		for (j=(i-1);j < n;j++)
			anorm += NrcAbs(a[i][j]);
	nn=n-1;
	t=0.0;
	while (nn >= 0) {
		its=0;
		do {
			for (l=nn;l>=1;l--) {
				s=NrcAbs(a[l-1][l-1])+NrcAbs(a[l][l]);
				if (s == 0.0) s=anorm;
				if ((float)(NrcAbs(a[l][l-1]) + s) == s) break;
			}
			x=a[nn][nn];
			if (l == nn) {
				wr[nn]=x+t;
				wi[nn--]=0.0;
			} else {
				y=a[nn-1][nn-1];
				w=a[nn][nn-1]*a[nn-1][nn];
				if (l == (nn-1)) {
					p=0.5*(y-x);
					q=p*p+w;
					z=sqrt(NrcAbs(q));
					x += t;
					if (q >= 0.0) {
						z=p+NrcSign(z,p);
						wr[nn-1]=wr[nn]=x+z;
						if (z) wr[nn]=x-w/z;
						wi[nn-1]=wi[nn]=0.0;
					} else {
						wr[nn-1]=wr[nn]=x+p;
						wi[nn-1]= -(wi[nn]=z);
					}
					nn -= 2;
				} else {
					if (its == 30) throw "Too many iterations in HseQr";
					if (its == 10 || its == 20) {
						t += x;
						for (i=0;i<=nn;i++) a[i][i] -= x;
						s=NrcAbs(a[nn][nn-1])+NrcAbs(a[nn-1][nn-2]);
						y=x=0.75*s;
						w = -0.4375*s*s;
					}
					++its;
					for (m=(nn-2);m>=l;m--) {
						z=a[m][m];
						r=x-z;
						s=y-z;
						p=(r*s-w)/a[m+1][m]+a[m][m+1];
						q=a[m+1][m+1]-z-r-s;
						r=a[m+2][m+1];
						s=NrcAbs(p)+NrcAbs(q)+NrcAbs(r);
						p /= s;
						q /= s;
						r /= s;
						if (m == l) break;
						u=NrcAbs(a[m][m-1])*(NrcAbs(q)+NrcAbs(r));
						v=NrcAbs(p)*(NrcAbs(a[m-1][m-1])+NrcAbs(z)+NrcAbs(a[m+1][m+1]));
						if ((float)(u+v) == v) break;
					}
					for (i=m+2;i<=nn;i++) {
						a[i][i-2]=0.0;
						if  (i != (m+2)) a[i][i-3]=0.0;
					}
					for (k=m;k<=nn-1;k++) {
						if (k != m) {
							p=a[k][k-1];
							q=a[k+1][k-1];
							r=0.0;
							if (k != (nn-1)) r=a[k+2][k-1];
							if ((x=NrcAbs(p)+NrcAbs(q)+NrcAbs(r))) {
								p /= x;
								q /= x;
								r /= x;
							}
						}
						if ((s=NrcSign(sqrt(p*p+q*q+r*r),p))) {
							if (k == m) {
								if (l != m)
								a[k][k-1] = -a[k][k-1];
							} else
								a[k][k-1] = -s*x;
							p += s;
							x=p/s;
							y=q/s;
							z=r/s;
							q /= p;
							r /= p;
							for (j=k;j<=nn;j++) {
								p=a[k][j]+q*a[k+1][j];
								if (k != (nn-1)) {
									p += r*a[k+2][j];
									a[k+2][j] -= p*z;
								}
								a[k+1][j] -= p*y;
								a[k][j] -= p*x;
							}
							mmin = nn<k+3 ? nn : k+3;
							for (i=l;i<=mmin;i++) {
								p=x*a[i][k]+y*a[i][k+1];
								if (k != (nn-1)) {
									p += z*a[i][k+2];
									a[i][k+2] -= p*r;
								}
								a[i][k+1] -= p*q;
								a[i][k] -= p;
							}
						}
					}
				}
			}
		} while (l < nn-1);
	}
}

template<class H,class W,class Z>
void Hseqr(H& h,int32  n,W& wr,W& wi,Z& z)
{
	int32  itn,its,i,j,k,l, m,nr,p1;
	float_t ave,cs,disc,ovfl,s,smlnum,sn,sum,tmp;
	float_t h00,h10,h11,h12,h21,h22,h33,h33s,h43h34,h44,h44s;
	float_t t1,t2,t3,tst1,unfl;
	float_t v[3];
	float_t dat1,dat2,ulp;

    dat1 = 0.75;
    dat2 = -0.4375;


	// Quick return if possible
    if(n==0)
		return;
    if(n==1)
    {
        wr[0] = h[0][0];
        wi[0] = 0;
        return;
    }

	ulp = std::numeric_limits<float_t>::epsilon();
	unfl = (std::numeric_limits<float_t>::min)();
    ovfl = (std::numeric_limits<float_t>::max)();
	smlnum = unfl*(n/ulp);


    for(j = 0; j < n-2; j++)
        for(i = j+2; i < n; i++)
            h[i][j] = 0;

	itn = 30*n;
	i = n-1;

	while(true)
    {
        l = 0;
        if(i < 0)
        {
            return;
        }
        for(its = 0; its < itn; its++)
        {
            for(k = i; k > l; k--)
            {
                tst1 = NrcAbs(h[k-1][k-1])+NrcAbs(h[k][k]);
                if( tst1==0 )
					tst1 = hsnorm(h,l,i);

                if(NrcAbs(h[k][k-1])<= NrcMax(ulp*tst1, smlnum))
                    break;
            }
            l = k;
            if(l>0)
                h[l][l-1] = 0;
            if(l >= (i-1))
                break;
            if( its==10||its==20 )
            {
                s = NrcAbs(h[i][i-1])+ NrcAbs(h[i-1][i-2]);
                h44 = dat1*s+h[i][i];
                h33 = h44;
                h43h34 = dat2*s*s;
            }
            else
            {
                h44 = h[i][i];
                h33 = h[i-1][i-1];
                h43h34 = h[i][i-1] * h[i-1][i];
                s = h[i-1][i-2] * h[i-1][i-2];
                disc = (h33-h44)*0.5;
                disc = disc*disc+h43h34;
                if( disc>0 )
                {
                    disc = sqrt(disc);
                    ave = 0.5*(h33 + h44);
                    if( NrcAbs(h33) - NrcAbs(h44)>0 )
                    {
                        h33 = h33*h44-h43h34;
                        h44 = h33/(NrcSign(disc, ave)+ave);
                    }
                    else
                    {
                        h44 = NrcSign(disc, ave)+ave;
                    }
                    h33 = h44;
                    h43h34 = 0;
                }
            }
			for(m = i-2; m >= l; m--)
            {
                h11 = h[m][m];
                h22 = h[m+1][m+1];
                h21 = h[m+1][m];
                h12 = h[m][m+1];
                h44s = h44-h11;
                h33s = h33-h11;
                v[0] = (h33s*h44s-h43h34)/h21+h12;
                v[1] = h22-h11-h33s-h44s;
                v[2] = h[m+2][m+1];
                s = NrcAbs(v[0])+NrcAbs(v[1])+NrcAbs(v[2]);
                v[0] /= s;
                v[1] /= s;
                v[2] /= s;

                if( m==l )
                    break;
                h00 = h[m-1][m-1];
                h10 = h[m][m-1];
                tst1 = NrcAbs(v[0])*(NrcAbs(h00)+NrcAbs(h11)+NrcAbs(h22));
                if( NrcAbs(h10)*(NrcAbs(v[1])+NrcAbs(v[2]))<=ulp*tst1 )
                    break;
            }
            for(k = m; k < i; k++)
            {
                nr = NrcMin(3, i-k+1);
                if(k>m)
                {
                    for(p1 = 0; p1 < nr; p1++)
                        v[p1] = h[k+p1][k-1];
                }
                reflec(v, nr, t1);
				if(k>m)
                {
                    h[k][k-1] = v[0];
                    h[k+1][k-1] = 0;
                    if(k<i-1)
                        h[k+2][k-1] = 0;
                }
                else if( m>l )
					h[k][k-1] = -h[k][k-1];

                t2 = t1*v[1];
				if(nr==3)
                {
					t3 = t1*v[2];
                    for(j = k; j < n; j++)
                    {
                        sum = h[k][j] + v[1]*h[k+1][j]+v[2]*h[k+2][j];
                        h[k][j] -= sum*t1;
                        h[k+1][j] -= sum*t2;
                        h[k+2][j] -= sum*t3;
                    }
                    for(j = 0; j <= NrcMin(k+3,i); j++)
                    {
                        sum = h[j][k] + v[1]*h[j][k+1]+v[2]*h[j][k+2];
						h[j][k] -= sum*t1;
                        h[j][k+1] -=sum*t2;
                        h[j][k+2] -=sum*t3;
                    }
					for(j = 0; j < n; j++)
                    {
                        sum = z[j][k] + v[1]*z[j][k+1]+v[2]*z[j][k+2];
                        z[j][k] -= sum*t1;
						z[j][k+1] -= sum*t2;
                        z[j][k+2] -= sum*t3;
                    }
                }
                else if(nr==2)
                {
                    for(j = k; j < n; j++)
                    {
                        sum = h[k][j] +v[1]*h[k+1][j];
                        h[k][j] -= sum*t1;
                        h[k+1][j] -= sum*t2;
                    }
                    for(j = 0; j <= i; j++)
                    {
                        sum = h[j][k]+v[1]*h[j][k+1];
                        h[j][k] -= sum*t1;
                        h[j][k+1] -= sum*t2;
                    }
                    for(j = 0; j < n; j++)
                    {
                        sum = z[j][k]+v[1]*z[j][k+1];
                        z[j][k] -= sum*t1;
                        z[j][k+1] -= sum*t2;
                    }
                }
            }
        }
        if(its ==itn)
            throw "Hesqr failed!";

        if( l==i )
        {
			wr[i] = h[i][i];
            wi[i] = 0;
        }
        else if( l==i-1 )
        {
            eig2x2(h[i-1][i-1], h[i-1][i],
				h[i][i-1], h[i][i],
				wr[i-1], wi[i-1],wr[i], wi[i], cs, sn);
			//Rotation à gauche
			for(k = i + 1;k < n;++k)
			{
				tmp = h[i][k];
				h[i][k] = tmp * cs - h[i-1][k] * sn;
				h[i-1][k] = tmp * sn + h[i-1][k] * cs;
			}
			//Rotation à droite
			for(k = 0;k < i-1;++k)
			{
				tmp = h[k][i];
				h[k][i] = tmp * cs - h[k][i-1] * sn;
				h[k][i-1] = tmp * sn + h[k][i-1] * cs;
			}
			for(k = 0;k < n;++k)
			{
				tmp = z[k][i];
				z[k][i] = tmp * cs - z[k][i-1] * sn;
				z[k][i-1] = tmp * sn + z[k][i-1] * cs;
			}

		}
        itn = itn-its;
        i = l-1;
	}

    j = 0;
    while(j < n)
    {
        if( wi[j]==0 )
        {
            for(i = j+1; i < n; i++)
            {
                h[i][j] = 0;
            }
            j++;
        }
        else
        {
            for(i = j+2; i < n; i++)
            {
                h[i][j] = 0;
				h[i][j+1] = 0;
            }
            j += 2;
        }
    }
}
} // nrc
#endif
