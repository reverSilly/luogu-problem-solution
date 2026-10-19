#include<bits/stdc++.h>
using namespace std;

// 独立基准：显式建图后 DFS 数连通块，与标程的并查集路径不同。
int n,m;
vector<int> g[1005];
bool vis[1005];

void dfs(int u)
{
	vis[u]=1;
	for(int v:g[u])
		if(!vis[v])
			dfs(v);
}

int main()
{
	while(cin>>n&&n)
	{
		cin>>m;
		for(int i=0;i<n;++i)
		{
			g[i].clear();
			vis[i]=0;
		}
		for(int j=0,u,v;j<m;++j)
		{
			cin>>u>>v;--u;--v;
			g[u].push_back(v);
			g[v].push_back(u);
		}
		int c=0;
		for(int i=0;i<n;++i)
			if(!vis[i])
			{
				++c;
				dfs(i);
			}
		cout<<c-1<<'\n';
	}
	return 0;
}
