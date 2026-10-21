//
// P1608 暴力（对拍用）：先求最短路长, 再 DFS 枚举所有点序列恰好等于最短路的路径
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int inf{1'000'000'000};
int n,m,w[12][12],dis[12];
bool used[12];
i64 way;

void dfs(int u,int cost)
{
	if(cost>dis[n-1])
		return;
	if(u==n-1)
	{
		if(cost==dis[n-1])
			++way;
		return;
	}
	for(int v=0;v<n;++v)
		if(!used[v]&&v!=u&&w[u][v]<inf)
		{
			used[v]=true;
			dfs(v,cost+w[u][v]);
			used[v]=false;
		}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<n;++j)
			w[i][j]=inf;
		w[i][i]=0;
	}
	for(int i=0,u,v,c;i<m;++i)
	{
		cin>>u>>v>>c;
		--u;--v;
		if(u!=v)
			w[u][v]=min(w[u][v],c);
	}
	for(int i=0;i<n;++i)
		dis[i]=w[0][i];
	vector<bool>vis(n,false);
	for(int it=0;it<n;++it)
	{
		int u=-1;
		for(int i=0;i<n;++i)
			if(!vis[i]&&dis[i]<inf&&(u==-1||dis[i]<dis[u]))
				u=i;
		if(u==-1)
			break;
		vis[u]=true;
		for(int v=0;v<n;++v)
			if(dis[u]+w[u][v]<dis[v])
				dis[v]=dis[u]+w[u][v];
	}
	if(dis[n-1]>=inf)
	{
		cout<<"No answer\n";
		return 0;
	}
	used[0]=true;
	dfs(0,0);
	cout<<dis[n-1]<<' '<<way<<'\n';
	return 0;
}
