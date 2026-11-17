// 题目：P2883 [USACO07MAR] Cow Traffic S
// 边恒 u<v ⇒ 编号就是拓扑序。g[u]=u→N 的路径数（倒序递推），f[u]=源点→u 的路径数（正序递推）。
// 边 (u,v) 的流量 = f[u]*g[v]。只给 g>0 的"活点"算 f：死胡同分量的路径数可以是天文数字，
// 而活点上的 f、g 都被答案（≤2^31-1）约束住，i64 可证不溢出。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{5005},mxm{50005};

int n,m,indeg[mxn];
i64 f[mxn],g[mxn];
vector<int>graph[mxn];
struct{int u,v;}E[mxm];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<m;++i)
	{
		cin>>E[i].u>>E[i].v;--E[i].u;--E[i].v;
		graph[E[i].u].push_back(E[i].v);	// 重边保留：每条边是独立的路径选择
		++indeg[E[i].v];
	}
	g[n-1]=1;
	for(int u=n-2;u>=0;--u)
		for(int v:graph[u])
			g[u]+=g[v];
	for(int u=0;u<n;++u)
	{
		if(!g[u])	// 死点不算 f，防止死胡同分量路径数爆炸
			continue;
		if(!indeg[u])
			f[u]=1;
		for(int v:graph[u])
			if(g[v])
				f[v]+=f[u];
	}
	i64 ans=0;
	for(int i=0;i<m;++i)
		ans=max(ans,f[E[i].u]*g[E[i].v]);
	cout<<ans<<'\n';
	return 0;
}