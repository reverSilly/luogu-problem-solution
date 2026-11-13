#include<bits/stdc++.h>
using namespace std;

double aT[4],aB[4],dT[4],dB[4];

int main()
{
	double x1,y1,x2,y2;
	cin>>x1>>y1>>x2>>y2;
	aT[0]=y1+0.5;aB[0]=y2+0.5;dT[0]=0.5-x1;dB[0]=0.5-x2;
	aT[1]=0.5-x1;aB[1]=0.5-x2;dT[1]=0.5-y1;dB[1]=0.5-y2;
	aT[2]=0.5-y1;aB[2]=0.5-y2;dT[2]=x1+0.5;dB[2]=x2+0.5;
	aT[3]=x1+0.5;aB[3]=x2+0.5;dT[3]=y1+0.5;dB[3]=y2+0.5;
	double ans=1e18;
	for(int f1=0;f1<4;++f1)
		for(int dir=-1;dir<=1;dir+=2)
			for(int k=1;k<=4;++k)
			{
				int fk=((f1+dir*(k-1))%4+4)%4;
				double uT=dir>0?aT[f1]:1-aT[f1];
				double uB=dir>0?aB[fk]:1-aB[fk];
				double vT=1+dT[f1],vB=-dB[fk];
				double du=(k-1+uB)-uT;
				double dv=vT-vB;
				double t1=(vT-1)/dv,t0=vT/dv;
				double u1=uT+t1*du,u0=uT+t0*du;
				bool ok=(u1>=-1e-12&&u1<=1+1e-12&&u0>=k-1-1e-12&&u0<=k+1e-12);
				if(ok)
					ans=min(ans,hypot(du,dv));
			}
	printf("%.3f\n",ans);
	return 0;
}
