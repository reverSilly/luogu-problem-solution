//
// 题目：P1851 好朋友
// https://www.luogu.com.cn/problem/P1851
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1000005};

int d[mxn];

int spd(int x)
{
	if(x<mxn)
		return d[x];
	int s=1;
	for(int i=2;(long long)i*i<=x;++i)
	{
		if(x%i==0)
		{
			s+=i;
			if(i!=x/i)
				s+=x/i;
		}
	}
	return s;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	for(int i=1;i<mxn;++i)
		for(int j=i+i;j<mxn;j+=i)
			d[j]+=i;
	int s;
	cin>>s;
	for(int a=s;;++a)
	{
		int b=spd(a);
		if(b!=a&&spd(b)==a)
		{
			cout<<a<<' '<<b<<'\n';
			return 0;
		}
	}
	return 0;
}
