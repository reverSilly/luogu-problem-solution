//
// 题目：P1873 [COCI 2011/2012 #5] EKO / 砍树
// https://www.luogu.com.cn/problem/P1873
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1000005};

int h[mxn],n;
i64 need;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>need;
	int mx=0;
	for(int i=0;i<n;++i)
	{
		cin>>h[i];
		mx=max(mx,h[i]);
	}
	int ans=0;
	for(int H=mx;H>=0;--H)
	{
		i64 s=0;
		for(int i=0;i<n;++i)
			if(h[i]>H)
				s+=h[i]-H;
		if(s>=need)
		{
			ans=H;
			break;
		}
	}
	cout<<ans<<'\n';
	return 0;
}
