// 题目：P3074 [USACO13FEB] Milk Scheduling S
// 无限工人 ⇒ 最短总时间 = DAG 点权最长路；拓扑序上 DP f[v]=t[v]+max f[u]
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{10005},mxm{50005};

int n,m,t[mxn],deg[mxn];
i64 f[mxn];	// f[v] 先存"前驱完成时刻的最大值"，出队时 +=t[v] 变成自身完成时刻
vector<int>graph[mxn];
int q[mxn],qh,qt;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>t[i];
	for(int i=0,u,v;i<m;++i)
	{
		cin>>u>>v;--u;--v;
		graph[u].push_back(v);
		++deg[v];
	}
	for(int i=0;i<n;++i)
		if(!deg[i])
			q[qt++]=i;
	i64 ans=0;
	while(qh<qt)
	{
		int u=q[qh++];
		f[u]+=t[u];	// 此刻 f[u] 已是最终完成时刻（所有前驱都处理过）
		ans=max(ans,f[u]);
		for(int v:graph[u])
		{
			f[v]=max(f[v],f[u]);
			if(!--deg[v])
				q[qt++]=v;
		}
	}
	cout<<ans<<'\n';
	return 0;
}
