//
// 题目：P1843 奶牛晒衣服
// https://www.luogu.com.cn/problem/P1843
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{500005};

int n,a,b;
i64 w[mxn];

bool dry(i64 t)
{
	i64 need=0;
	i64 nat=(i64)a*t;
	for(int i=0;i<n;++i)
	{
		if(w[i]>nat)
		{
			need+=(w[i]-nat+b-1)/b;
			if(need>t)
				return false;
		}
	}
	return true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>a>>b;
	i64 hi=0;
	for(int i=0;i<n;++i)
	{
		cin>>w[i];
		if(w[i]>hi)
			hi=w[i];
	}
	i64 lo=0;
	while(lo<hi)
	{
		i64 mid=(lo+hi)/2;
		if(dry(mid))
			hi=mid;
		else
			lo=mid+1;
	}
	cout<<lo<<'\n';
	return 0;
}
