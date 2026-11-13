#include<bits/stdc++.h>
using namespace std;
static mt19937 rng(7);
double X1,Y1,X2,Y2;
int main()
{
	uniform_real_distribution<double>U(-0.5,0.5);
	int cnt[8][3]{}; // [k][f1] winners
	int winK[5]{};
	for(int tc=0;tc<20000;++tc)
	{
		X1=U(rng);Y1=U(rng);X2=U(rng);Y2=U(rng);
		double aT[4],aB[4],dT[4],dB[4];
		aT[0]=Y1+0.5;aB[0]=Y2+0.5;dT[0]=0.5-X1;dB[0]=0.5-X2;
		aT[1]=0.5-X1;aB[1]=0.5-X2;dT[1]=0.5-Y1;dB[1]=0.5-Y2;
		aT[2]=0.5-Y1;aB[2]=0.5-Y2;dT[2]=X1+0.5;dB[2]=X2+0.5;
		aT[3]=X1+0.5;aB[3]=X2+0.5;dT[3]=Y1+0.5;dB[3]=Y2+0.5;
		double ans=1e18;int bk=0,bf=0;
		for(int f1=0;f1<4;++f1)
			for(int dir=-1;dir<=1;dir+=2)
				for(int k=1;k<=4;++k)
				{
					int fk=((f1+dir*(k-1))%4+4)%4;
					double uT=dir>0?aT[f1]:1-aT[f1];
					double uB=dir>0?aB[fk]:1-aB[fk];
					double vT=1+dT[f1],vB=-dB[fk];
					double du=(k-1+uB)-uT,dv=vT-vB;
					double t1=(vT-1)/dv,t0=vT/dv;
					double u1=uT+t1*du,u0=uT+t0*du;
					bool ok=(u1>=-1e-12&&u1<=1+1e-12&&u0>=k-1-1e-12&&u0<=k+1e-12);
					double w=hypot(du,dv);
					if(ok&&w<ans-1e-12){ans=w;bk=k;bf=f1;}
				}
		winK[bk]++;
	}
	for(int k=1;k<=4;++k)
		printf("k=%d wins %d\n",k,winK[k]);
	return 0;
}
