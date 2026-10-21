//
// 题目：P1842 [USACO05NOV] 奶牛玩杂技
// https://www.luogu.com.cn/problem/P1842
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{50005};

struct cow
{
	int w,s;
}c[mxn];

bool cmp(cow a,cow b)
{
	return a.w+a.s<b.w+b.s;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int n;
	cin>>n;
	for(int i=0;i<n;++i)
		cin>>c[i].w>>c[i].s;
	sort(c,c+n,cmp);
	i64 above=0,ans=-(i64)1e18;
	for(int i=0;i<n;++i)
	{
		ans=max(ans,above-c[i].s);
		above+=c[i].w;
	}
	cout<<ans<<'\n';
	return 0;
}
