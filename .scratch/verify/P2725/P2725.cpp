// 题目：P2725 [USACO3.1] 邮票 Stamps
// f[v]=凑出 v 的最少邮票数（完全背包求最小个数）。
// 答案 = 第一个 f[v]>k 的 v 减 1；f[v]≥ceil(v/maxa) ⇒ 第一个缺口 ≤ k*maxa+1。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxv{2000005},inf{1145141919};

int k,n,a[55],f[mxv];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>k>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];
	sort(a,a+n);
	n=unique(a,a+n)-a;	// 去重：重复面值不改变可表示性
	int mx=k*a[n-1]+1,ans=mx;	// 第一个缺口必不超过 mx
	for(int v=1;v<=mx;++v)
	{
		f[v]=inf;
		for(int i=0;i<n&&a[i]<=v;++i)
			f[v]=min(f[v],f[v-a[i]]+1);
		if(f[v]>k)
		{
			ans=v-1;
			break;
		}
	}
	cout<<ans<<'\n';
	return 0;
}