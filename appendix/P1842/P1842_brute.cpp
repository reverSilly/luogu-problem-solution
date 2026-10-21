//
// 题目：P1842 [USACO05NOV] 奶牛玩杂技 暴力（全排列）
// https://www.luogu.com.cn/problem/P1842
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int n;
	cin>>n;
	i64 w[12],s[12];
	int p[12];
	for(int i=0;i<n;++i)
	{
		cin>>w[i]>>s[i];
		p[i]=i;
	}
	i64 best=(i64)1e18;
	do
	{
		i64 above=0,cur=-(i64)1e18;
		for(int i=0;i<n;++i)
		{
			cur=max(cur,above-s[p[i]]);
			above+=w[p[i]];
		}
		best=min(best,cur);
	}
	while(next_permutation(p,p+n));
	cout<<best<<'\n';
	return 0;
}
