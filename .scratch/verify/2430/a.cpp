// 题目：P2430 严酷的训练
// 思路：WKY 耗时 = (b/a)*老王耗时；每题是独立物品(重量 k*t[p], 价值 q)
//       0/1 背包：f[c]=总耗时<=c 的最大奖励，容量倒序滚动
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{105},mxt{5005};

int n,m,T,a,b,t[mxn],v[mxn],f[mxt];
i64 w[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>a>>b;
	cin>>m>>n;
	for(int i=1;i<=n;++i)cin>>t[i];
	int k=b/a;
	for(int j=0;j<m;++j)
	{
		int p,q;cin>>p>>q;
		w[j]=(i64)k*t[p];
		v[j]=q;
	}
	cin>>T;
	// 0/1 knapsack: f[c]=max reward with total time <= c
	for(int j=0;j<m;++j)
	{
		if(w[j]>T)continue;
		int ww=(int)w[j];
		for(int c=T;c>=ww;--c)
			f[c]=max(f[c],f[c-ww]+v[j]);
	}
	cout<<f[T]<<'\n';
	return 0;
}
