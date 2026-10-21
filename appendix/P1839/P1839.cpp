//
// 题目：P1839 Play with Power
// https://www.luogu.com.cn/problem/P1839
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxa{10005};
constexpr int mxb{40};

i64 n;
int lim,maxb;
int f[mxa][mxb];
int ray[mxb];

bool ok(i64 a,int b)
{
	if(a<1||b<1)
		return false;
	if(b==1)
		return a<=n;
	if(a==1)
		return true;
	i64 r=1;
	for(int i=0;i<b;++i)
	{
		if(r>n/a)
			return false;
		r*=a;
	}
	return r<=n;
}

int biga(i64 a)
{
	return ((n-a)&1)?2:0;
}

void build()
{
	lim=0;
	while((i64)(lim+1)*(lim+1)<=n)
		++lim;
	for(int b=mxb-2;b>=1;--b)
	{
		for(int a=lim;a>=2;--a)
		{
			if(!ok(a,b))
				continue;
			bool any=false,hasl=false,hasd=false;
			if(ok(a+1,b))
			{
				any=true;
				int r=b==1&&a+1>lim?biga(a+1):f[a+1][b];
				if(r==0)
					hasl=true;
				else if(r==1)
					hasd=true;
			}
			if(b+1<mxb&&ok(a,b+1))
			{
				any=true;
				int r=f[a][b+1];
				if(r==0)
					hasl=true;
				else if(r==1)
					hasd=true;
			}
			if(!any)
				f[a][b]=0;
			else if(hasl)
				f[a][b]=2;
			else if(hasd)
				f[a][b]=1;
			else
				f[a][b]=0;
		}
	}
	maxb=0;
	i64 p2=1;
	while(p2<=n/2)
	{
		p2*=2;
		++maxb;
	}
	ray[maxb+1]=1;
	for(int b=maxb;b>=1;--b)
	{
		bool hasl=false,hasd=false;
		if(ok(2,b))
		{
			int r=b==1&&2>lim?biga(2):f[2][b];
			if(r==0)
				hasl=true;
			else if(r==1)
				hasd=true;
		}
		if(ray[b+1]==0)
			hasl=true;
		else if(ray[b+1]==1)
			hasd=true;
		if(hasl)
			ray[b]=2;
		else if(hasd)
			ray[b]=1;
		else
			ray[b]=0;
	}
}

void say(int o)
{
	if(o==2)
		cout<<"Masha\n";
	else if(o==0)
		cout<<"Stas\n";
	else
		cout<<"Missing\n";
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int t;
	cin>>n>>t;
	build();
	while(t--)
	{
		int a,b;
		cin>>a>>b;
		if(a==1)
		{
			if(b>maxb)
				say(1);
			else
				say(ray[b]);
		}
		else if(b==1&&a>lim)
			say(biga(a));
		else
			say(f[a][b]);
	}
	return 0;
}
