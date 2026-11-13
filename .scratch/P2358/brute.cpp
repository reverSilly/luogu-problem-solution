// 独立暴力: 路径必经过顶面边界上的点 P 与底面边界上的点 Q
// 长度 = |T-P| + 侧面柱面测地线(P,Q) + |Q-B|
// 在 (u,v) in [0,4)^2 上做网格搜索 + 局部细化, 精度极高
#include<bits/stdc++.h>
using namespace std;

double x1,y1,x2,y2;

// 侧面按 u in [0,4] 参数化: 0:E(x=.5) 1:N(y=.5) 2:W(x=-.5) 3:S(y=-.5)
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
	double a=hypot(px-x1,py-y1);
	double b=hypot(qx-x2,qy-y2);
	double d=u-v;
	while(d>2)d-=4;
	while(d<-2)d+=4;
	double c=sqrt(d*d+1);
	return a+b+c;
}

int main()
{
	cin>>x1>>y1>>x2>>y2;
	double bu=0,bv=0,best=1e18;
	int M=300;
	for(int i=0;i<M;++i)
		for(int j=0;j<M;++j)
		{
			double u=4.0*i/M,v=4.0*j/M;
			double w=eval(u,v);
			if(w<best){best=w;bu=u;bv=v;}
		}
	double step=4.0/M;
	for(int it=0;it<60;++it)
	{
		step*=0.7;
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
	printf("%.6f\n",best);
	return 0;
}
