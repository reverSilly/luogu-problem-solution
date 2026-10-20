// 题目：P1550 [USACO08OCT] Watering Hole G
// 暴力基准：枚举候选边(井边 + 水管边)的所有子集,找使所有田都与源点连通的方案最小花费
// 仅用于小规模对拍,不能提交
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{12};
constexpr int inf{1145141919};

int n;
int W[mxn],P[mxn][mxn];
int eu[mxn*mxn],ev[mxn*mxn],ew[mxn*mxn];
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

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>W[i];
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>P[i][j];

	int me=0;
	for(int i=0;i<n;++i)                          // 井: 源点 n <-> i
	{
		eu[me]=n;ev[me]=i;ew[me]=W[i];++me;
	}
	for(int i=0;i<n;++i)                          // 水管: i <-> j
		for(int j=i+1;j<n;++j)
		{
			eu[me]=i;ev[me]=j;ew[me]=P[i][j];++me;
		}

	int best=inf;
	for(int mask=0;mask<(1<<me);++mask)
	{
		for(int i=0;i<=n;++i)
			f[i]=i;
		int cost=0;
		for(int i=0;i<me;++i)
			if(mask>>i&1)
			{
				int a=getrt(eu[i]),b=getrt(ev[i]);
				if(a!=b)
					f[a]=b;
				cost+=ew[i];
			}
		bool ok=true;
		for(int i=0;i<n&&ok;++i)                  // 每块田都必须与源点连通
			if(getrt(i)!=getrt(n))
				ok=false;
		if(ok)
			best=min(best,cost);
	}
	cout<<best<<'\n';
	return 0;
}
