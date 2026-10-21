//
// 题目：P1862 输油管道问题
// https://www.luogu.com.cn/problem/P1862
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{10005};

int n;
int y[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	int lo=1145141919,hi=-1145141919;
	for(int i=0;i<n;++i)
	{
		int x;
		cin>>x>>y[i];
		lo=min(lo,y[i]);
		hi=max(hi,y[i]);
	}
	i64 ans=4e18;
	for(int p=lo;p<=hi;++p)
	{
		i64 cur=0;
		for(int i=0;i<n;++i)
			cur+=abs(y[i]-p);
		ans=min(ans,cur);
	}
	cout<<ans<<'\n';
	return 0;
}
