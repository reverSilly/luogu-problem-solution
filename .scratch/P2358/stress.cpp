// 对拍: 解析解 vs 独立暴力(边界点二维寻优)
#include<bits/stdc++.h>
using namespace std;

static mt19937 rng(12345);

double X1,Y1,X2,Y2;
double aT[4],aB[4],dT[4],dB[4];

double sol()
{
	aT[0]=Y1+0.5;aB[0]=Y2+0.5;dT[0]=0.5-X1;dB[0]=0.5-X2;
	aT[1]=0.5-X1;aB[1]=0.5-X2;dT[1]=0.5-Y1;dB[1]=0.5-Y2;
	aT[2]=0.5-Y1;aB[2]=0.5-Y2;dT[2]=X1+0.5;dB[2]=X2+0.5;
	aT[3]=X1+0.5;aB[3]=X2+0.5;dT[3]=Y1+0.5;dB[3]=Y2+0.5;
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
	return ans;
}

void edgept(double u,double&x,double&y)
{
	int f=(int)floor(u)%4;
	double a=u-floor(u);
	if(f==0){x=0.5;y=a-0.5;}
	else if(f==1){x=0.5-a;y=0.5;}
	else if(f==2){x=-0.5;y=0.5-a;}
	else {x=a-0.5;y=-0.5;}
}

double eval(double u,double v)
{
	double px,py,qx,qy;
	edgept(u,px,py);
	edgept(v,qx,qy);
	double a=hypot(px-X1,py-Y1);
	double b=hypot(qx-X2,qy-Y2);
	double d=u-v;
	while(d>2)d-=4;
	while(d<-2)d+=4;
	return a+b+sqrt(d*d+1);
}

double brute()
{
	double bu=0,bv=0,best=1e18;
	int M=120;
	for(int i=0;i<M;++i)
		for(int j=0;j<M;++j)
		{
			double u=4.0*i/M,v=4.0*j/M;
			double w=eval(u,v);
			if(w<best){best=w;bu=u;bv=v;}
		}
	double step=4.0/M;
	for(int it=0;it<70;++it)
	{
		step*=0.75;
		for(int di=-1;di<=1;++di)
			for(int dj=-1;dj<=1;++dj)
			{
				double u=bu+di*step,v=bv+dj*step;
				if(u<0||u>=4||v<0||v>=4)
					continue;
				double w=eval(u,v);
				if(w<best){best=w;bu=u;bv=v;}
			}
	}
	return best;
}

int main()
{
	uniform_real_distribution<double>U(-0.5,0.5);
	double worst=0;
	int bad=0;
	for(int tc=0;tc<400;++tc)
	{
		X1=U(rng);Y1=U(rng);X2=U(rng);Y2=U(rng);
		// 一半样例取边界上的点(容易出极端情况)
		if(tc%2==0)
		{
			if(tc%4==0)X1=0.5;
			else Y1=-0.5;
			if(tc%8<4)X2=-0.5;
			else Y2=0.5;
		}
		double a=sol(),b=brute();
		double diff=b-a;
		if(diff<-1e-6||fabs(diff)>2e-2)
		{
			++bad;
			printf("MISMATCH %.3f %.3f | sol=%.9f brute=%.9f diff=%.9f\n",X1,Y1,sol(),b,diff);
		}
		worst=max(worst,fabs(diff));
	}
	printf("done, bad=%d, worst |diff|=%.6f\n",bad,worst);
	return 0;
}
