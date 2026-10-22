// 题目：P2026 求一次函数解析式
// k=(y2-y1)/(x2-x1)，b=y1-k*x1，全程用分数精确计算
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
using i128=__int128_t;

i64 gcdl(i64 a,i64 b)
{
	if(a<0)
		a=-a;
	if(b<0)
		b=-b;
	while(b)
	{
		i64 t=a%b;
		a=b;
		b=t;
	}
	return a;
}

//把 p/q 约分并保证分母为正，返回分母
i64 norm(i64 &p,i64 &q)
{
	if(q<0)
	{
		p=-p;
		q=-q;
	}
	i64 g=gcdl(p,q);
	if(g)
	{
		p/=g;
		q/=g;
	}
	return q;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	i64 x1,y1,x2,y2;
	cin>>x1>>y1;
	cin>>x2>>y2;
	//斜率 k=kn/kd
	i64 kn=y2-y1,kd=x2-x1;
	norm(kn,kd);
	//截距 b=(y1*kd-kn*x1)/kd
	i128 bn=(i128)y1*kd-(i128)kn*x1;
	i64 bq=kd;
	i64 bp=(i64)bn;
	norm(bp,bq);
	cout<<"y=";
	if(kd==1)
		cout<<kn<<'x';
	else
		cout<<kn<<'/'<<kd<<'*'<<'x';
	if(bp!=0)
	{
		if(bp>0)
			cout<<'+';
		else
			cout<<'-';
		i64 ap=bp<0?-bp:bp;
		if(bq==1)
			cout<<ap;
		else
			cout<<ap<<'/'<<bq;
	}
	cout<<'\n';
	return 0;
}
