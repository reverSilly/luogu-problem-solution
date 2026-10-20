// 题目：P1547 [USACO05MAR] Out of Hay S
// 暴力基准：枚举所有边子集,取恰好 n-1 条且连通(生成树)的方案中"最大边权"的最小值
// 仅用于小规模对拍,不能提交
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{15};
constexpr int mxm{30};
constexpr int inf{1145141919};

int n,m;
int eu[mxm],ev[mxm],ew[mxm];
int f[mxn];

int getrt(int x)
{
	if(f[x]==x)
		return x;
	return f[x]=getrt(f[x]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<m;++i)
	{
		cin>>eu[i]>>ev[i]>>ew[i];
		--eu[i];--ev[i];
	}
	int best=inf;
	for(int mask=0;mask<(1<<m);++mask)
	{
		if(__builtin_popcount(mask)!=n-1)
			continue;
		for(int i=0;i<n;++i)
			f[i]=i;
		int mx=0;
		bool ok=true;
		for(int i=0;i<m;++i)
			if(mask>>i&1)
			{
				int a=getrt(eu[i]),b=getrt(ev[i]);
				if(a==b){ok=false;break;}             // 成环 => 不是生成树
				f[a]=b;
				mx=max(mx,ew[i]);
			}
		if(ok)
			best=min(best,mx);
	}
	cout<<best<<'\n';
	return 0;
}
