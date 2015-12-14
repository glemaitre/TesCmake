/// \file trevc.h
/// \brief Eigen system
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date September 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef NRC_TREVC_H
#define NRC_TREVC_H

#include <limits>
#include "typedef.h"
#include "nrcutil.h"
#include "lusol.h"

namespace nrc
{
template<class T,class VL,class VR>
void Trevc(const T& t,
     int32  n,int32  side,VL& vl,VR& vr)
{
	float_t wr,wi,tmpr,tmp1,tmp2,norm,dot;
	float_t b[4];
	float_t a[4][4];

	int32  j,k,ki,ip;
	bool skipflag,leftv,rightv;

    float_t* vec = new float_t [n];
	float_t* veci = new float_t [n];

	rightv = leftv = false;
	if(side & 1)
		rightv = true;
	if(side & 2)
		leftv = true;

	if( rightv )
    {
        ip = 0;
        for(ki = n-1; ki >= 0; ki--)
        {
            skipflag = false;
            if( ip==1 )
                skipflag = true;
            else if( ki > 0 && t[ki][ki-1]!=0)
                        ip = -1;
            if( !skipflag )
			{
                wr = t[ki][ki];
                if(ip==0)
					wi = 0;
				else
					wi = sqrt(NrcAbs(t[ki][ki-1]))*sqrt(NrcAbs(t[ki-1][ki]));

				// Real right eigenvector
				if( ip==0 )
				{
					for(k = 0; k < ki; k++)
						vec[k] = -t[k][ki];
					vec[ki] = 1;

	                j = ki-1;
					while(j >= 0)
                    {
						if(j <= 0 || t[j][j-1]==0)
						{
							//1x1
							vec[j] /= (t[j][j]-wr);
							//Update vec
							for(k = 0;k < j;++k)
								vec[k] -= vec[j] * t[k][j];
							j--;
						}
						else
						{
							tmpr = (t[j][j]-wr);
							tmp1 = t[j][j-1];
							tmp2 = t[j-1][j];

							norm = 1/(tmpr * tmpr - tmp1 * tmp2);
							tmp1 *= vec[j-1];
							tmp2 *= vec[j];
							vec[j] *= tmpr;
							vec[j] -= tmp1;
							vec[j] *= norm;

							vec[j-1] *= tmpr;
							vec[j-1] -=tmp2;
							vec[j-1] *= norm;

							//Update vec
							for(k = 0;k < (j-1);++k)
							{
								vec[k] -= vec[j] * t[k][j] +
										vec[j-1] * t[k][j-1];
							}
							j-=2;
						}
					}
					norm = (std::numeric_limits<float_t>::min)();
					for(j = 0;j < n; j++)
					{
						dot = 0;
						for(k = 0;k < ki; k++)
							dot += vr[j][k]*vec[k];
						/*vr[j][ki]*= vec[ki];*/
						vr[j][ki] += dot;
						if(NrcAbs(vr[j][ki]) > norm)
							norm = NrcAbs(vr[j][ki]);
					}
					norm = 1/norm;
					for(j = 0;j < n; j++)
						vr[j][ki] *= norm;
				}
				else
				{
                    if(NrcAbs(t[ki-1][ki])>= NrcAbs(t[ki][ki-1]))
                    {
                        vec[ki-1] = 1;
                        veci[ki] = wi/t[ki-1][ki];
                    }
                    else
                    {
                        vec[ki-1] = -wi/t[ki][ki-1];
                        veci[ki] = 1;
                    }
					//Update vec
					for(k = 0; k < ki-1; k++)
					{
						vec[k] = -t[k][ki-1]* vec[ki-1];
						veci[k] = -t[k][ki]* veci[ki];
					}
					vec[ki] = 0;
					veci[ki-1] = 0;


	                j = ki-2;
					while(j >= 0)
                    {
						if(j <= 0 || t[j][j-1]==0)
						{
							//1x1
							tmpr = t[j][j]-wr;
							norm = 1/(tmpr * tmpr + wi * wi);
							tmp1 = vec[j];
							tmp2 = veci[j];
							vec[j] = tmp1 * tmpr - tmp2 * wi;
							vec[j] *= norm;
							veci[j] = tmp2 * tmpr + tmp1 * wi;
							veci[j] *= norm;
							//Update vec
							for(k = 0;k < j;++k)
							{
								vec[k] -= vec[j] * t[k][j];
								veci[k] -= veci[j] * t[k][j];
							}
							j--;
						}
						else
						{
							//2x2
							a[0][0] = a[1][1] = (t[j][j]-wr);

							a[0][1] = t[j-1][j];
							a[0][2] = wi;
							a[0][3] = 0;

							a[1][0] = t[j][j-1];
							a[1][2] = 0;
							a[1][3] = wi;

							a[2][0] = -wi;
							a[2][1] = 0;
							a[2][2] = a[0][0];
							a[2][3] = a[0][1];

							a[3][0] = 0;
							a[3][1] = -wi;
							a[3][2] = a[1][0];
							a[3][3] = a[1][1];

							b[0] = vec[j-1];
							b[1] = vec[j];
							b[2] = veci[j-1];
							b[3] = veci[j];

							nrc::LuSol(a,4,b);
							vec[j-1] = b[0];
							vec[j] = b[1];
							veci[j-1] = b[2];
							veci[j] = b[3];
							//Update vec
							for(k = 0;k < j-1;++k)
							{
								vec[k] -= vec[j] * t[k][j] +
										vec[j-1] * t[k][j-1];
								veci[k] -= veci[j] * t[k][j] +
										veci[j-1] * t[k][j-1];
							}
							j-=2;
						}
					}
					norm = (std::numeric_limits<float_t>::min)();
					for(j = 0;j < n; j++)
					{
						dot = 0;
						for(k = 0;k < ki-1; k++)
							dot += vr[j][k]*vec[k];
						vr[j][ki-1]*= vec[ki-1];
						vr[j][ki-1] += dot;
						dot = 0;
						for(k = 0;k < ki-1; k++)
							dot += vr[j][k]*veci[k];
						vr[j][ki]*= veci[ki];
						vr[j][ki] += dot;

						tmpr = NrcAbs(vr[j][ki-1]) + NrcAbs(vr[j][ki]);
						if(tmpr > norm)
							norm = tmpr;
					}
					norm = 1/norm;
					for(j = 0;j < n; j++)
					{
						vr[j][ki-1] *= norm;
						vr[j][ki] *= norm;
					}
				}
			}
            if(ip==1)
                ip = 0;
            else if(ip==-1)
                ip = 1;
		}
	}
	if(leftv)
	{
        ip = 0;
        for(ki = 0; ki < n; ki++)
        {
            skipflag = false;
            if( ip==-1 )
                skipflag = true;
            else if( ki < n-1 && t[ki+1][ki]!=0)
                        ip = 1;
			if( !skipflag )
			{
                wr = t[ki][ki];
                if(ip==0)
					wi = 0;
				else
					wi = sqrt(NrcAbs(t[ki][ki+1]))*sqrt(NrcAbs(t[ki+1][ki]));
				// Real right eigenvector
				if( ip==0 )
				{
					for(k = ki+1; k < n; k++)
						vec[k] = -t[ki][k];
					vec[ki] = 1;

	                j = ki+1;
					while(j < n)
                    {
						if(j >= n-1 || t[j+1][j]==0)
						{
							//1x1
							tmp1 = 0;
							for(k = ki+1;k < j;++k)
								tmp1 +=t[k][j] * vec[k];
							vec[j] -= tmp1;
							vec[j] /=(t[j][j]-wr);
							j++;
						}
						else
						{
							//2x2
							tmp1 = 0;
							tmp2 = 0;
							for(k = ki+1;k < j;++k)
							{
								tmp1 +=t[k][j] * vec[k];
								tmp2 +=t[k][j+1] * vec[k];
							}
							vec[j] -= tmp1;
							vec[j+1] -= tmp2;

							tmpr = (t[j][j]-wr);
							tmp1 = t[j+1][j];
							tmp2 = t[j][j+1];

							norm = 1/(tmpr * tmpr - tmp1 * tmp2);
							tmp1 *= vec[j+1];
							tmp2 *= vec[j];

							vec[j] *= tmpr;
							vec[j] -= tmp1;
							vec[j] *= norm;

							vec[j+1] *= tmpr;
							vec[j+1] -=tmp2;
							vec[j+1] *= norm;

							j+=2;
						}
					}
					norm = (std::numeric_limits<float_t>::min)();
					for(j = 0;j < n; j++)
					{
						dot = 0;
						for(k = ki+1;k < n; k++)
							dot += vr[j][k]*vec[k];
						vr[j][ki] *= vec[ki];
						vr[j][ki] += dot;
						if(NrcAbs(vr[j][ki]) > norm)
							norm = NrcAbs(vr[j][ki]);
					}
					norm = 1/norm;
					for(j = 0;j < n; j++)
						vr[j][ki] *= norm;
				}
				else
				{
                    if(NrcAbs(t[ki][ki+1])>= NrcAbs(t[ki+1][ki]))
                    {
                        vec[ki] = wi/t[ki][ki+1];
						veci[ki+1] = 1;
                    }
                    else
                    {
						vec[ki] = 1;
                        veci[ki+1] = -wi/t[ki+1][ki];
                    }
					//Update vec
					for(k = ki+2; k < n; k++)
					{
						vec[k] = -t[ki][k]* vec[ki];
						veci[k] = -t[ki+1][k]* veci[ki+1];
					}
					vec[ki+1] = 0;
					veci[ki] = 0;

	                j = ki+2;
					while(j < n)
					{
						if(j >= n-1 || t[j+1][j]==0)
						{
							//1x1
							tmp1 = 0;
							tmp2 = 0;
							for(k = ki+2;k < j;++k)
							{
								tmp1 +=t[k][j] * vec[k];
								tmp2 +=t[k][j] * veci[k];
							}
							vec[j] -= tmp1;
							veci[j] -= tmp2;

							tmpr = t[j][j]-wr;
							norm = 1/(tmpr * tmpr + wi * wi);
							tmp1 = vec[j];
							tmp2 = veci[j];
							vec[j] = tmp1 * tmpr + tmp2 * wi;
							vec[j] *= norm;
							veci[j] = tmp2 * tmpr - tmp1 * wi;
							veci[j] *= norm;
							j++;
						}
						else
						{
							tmp1 = 0;
							tmp2 = 0;
							for(k = ki+2;k < j;++k)
							{
								tmp1 +=t[k][j] * vec[k];
								tmp2 +=t[k][j] * veci[k];
							}
							vec[j] -= tmp1;
							veci[j] -= tmp2;

							tmp1 = 0;
							tmp2 = 0;
							for(k = ki+2;k < j;++k)
							{
								tmp1 +=t[k][j+1] * vec[k];
								tmp2 +=t[k][j+1] * veci[k];
							}
							vec[j+1] -= tmp1;
							veci[j+1] -= tmp2;

							//2x2
							a[0][0] = a[1][1] = (t[j][j]-wr);

							a[0][1] = t[j][j+1];
							a[0][2] = -wi;
							a[0][3] = 0;

							a[1][0] = t[j+1][j];
							a[1][2] = 0;
							a[1][3] = -wi;

							a[2][0] = wi;
							a[2][1] = 0;
							a[2][2] = a[0][0];
							a[2][3] = a[0][1];

							a[3][0] = 0;
							a[3][1] = wi;
							a[3][2] = a[1][0];
							a[3][3] = a[1][1];

							b[0] = vec[j+1];
							b[1] = vec[j];
							b[2] = veci[j+1];
							b[3] = veci[j];

							nrc::LuSol(a,4,b);
							vec[j+1] = b[0];
							vec[j] = b[1];
							veci[j+1] = b[2];
							veci[j] = b[3];

							j+=2;
						}
					}
					norm = (std::numeric_limits<float_t>::min)();
					for(j = 0;j < n; j++)
					{
						dot = 0;
						for(k = ki+2;k < n; k++)
							dot += vr[j][k]*vec[k];
						vr[j][ki]*= vec[ki];
						vr[j][ki] += dot;

						dot = 0;
						for(k = ki+2;k < n; k++)
							dot += vr[j][k]*veci[k];
						vr[j][ki+1]*= veci[ki+1];
						vr[j][ki+1] += dot;

						tmpr = NrcAbs(vr[j][ki+1]) + NrcAbs(vr[j][ki]);
						if(tmpr > norm)
							norm = tmpr;
					}
					norm = 1/norm;
					for(j = 0;j < n; j++)
					{
						vr[j][ki] *= norm;
						vr[j][ki+1] *= norm;
					}
				}
			}
            if(ip==-1)
                ip = 0;
            else if(ip==1)
                ip = -1;
		}
	}
	delete[] vec;
	delete[] veci;
}
}
#endif
