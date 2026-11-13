// 题目：P2358 蚂蚁搬家
// 四个侧面剪开铺平成「长 4、宽 1」的带子；顶面/底面沿某个侧面的上/下边展开成单位正方形。
// 枚举 (展开入口侧面 f, 绕行方向 dir, 经过的侧面数 k)，看展开图上 T'->B' 的直线是否落在展开图内。
#include<bits/stdc++.h>
using namespace std;

double x1,y1,x2,y2;
double ofsT[4],ofsB[4],dt[4],db[4];   // 带内偏移 in[0,1]、到该侧面所在平面的距离

// 侧面编号 0:E(x=0.5) 1:N(y=0.5) 2:W(x=-0.5) 3:S(y=-0.5)
void ini()
{
	ofsT[0]=y1+0.5;ofsB[0]=y2+0.5;dt[0]=0.5-x1;db[0]=0.5-x2;
	ofsT[1]=0.5-x1;ofsB[1]=0.5-x2;dt[1]=0.5-y1;db[1]=0.5-y2;
	ofsT[2]=0.5-y1;ofsB[2]=0.5-y2;dt[2]=x1+0.5;db[2]=x2+0.5;
	ofsT[3]=x1+0.5;ofsB[3]=x2+0.5;dt[3]=y1+0.5;db[3]=y2+0.5;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>x1>>y1>>x2>>y2;
	ini();
	double ans=1e9;
	for(int f=0;f<4;++f)
		for(int dir=-1;dir<=1;dir+=2)
			for(int k=1;k<=3;++k)
			{
				int g=((f+dir*(k-1))%4+4)%4;
				// dir=-1 时带子坐标要整体镜像, 否则「落在展开图内」判断会错
				double ut=dir>0?ofsT[f]:1-ofsT[f];
				double ub=dir>0?ofsB[g]:1-ofsB[g];
				double vt=1+dt[f],vb=-db[g];
				double du=(k-1+ub)-ut,dv=vt-vb;
				// 直线穿过 v=1 必须在第 1 个侧面上, 穿过 v=0 必须在第 k 个侧面上
				double p=(vt-1)/dv,q=vt/dv;
				double u1=ut+p*du,u0=ut+q*du;
				if(u1<-1e-9||u1>1+1e-9)
					continue;
				if(u0<k-1-1e-9||u0>k+1e-9)
					continue;
				ans=min(ans,hypot(du,dv));
			}
	cout<<fixed<<setprecision(3)<<ans<<'\n';
	return 0;
}
