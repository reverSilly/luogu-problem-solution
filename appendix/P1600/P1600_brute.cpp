#include<bits/stdc++.h>
using namespace std;

int n,m;
vector<int> adj[5005];
int w[5005],par[5005],dep[5005],ans[5005];

void dfs(int u,int p,int d)
{
	par[u]=p;
	dep[u]=d;
	for(int v:adj[u])
		if(v!=p)
			dfs(v,u,d+1);
}

int main()
{
	cin>>n>>m;
	for(int i=0;i<n-1;++i)
	{
		int u,v;
		cin>>u>>v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	for(int i=1;i<=n;++i)
		cin>>w[i];
	dfs(1,0,0);
	for(int i=0;i<m;++i)
	{
		int s,t;
		cin>>s>>t;
		vector<int> path_s,path_t;
		int u=s,v=t;
		while(u!=v)
		{
			if(dep[u]>=dep[v])
			{
				path_s.push_back(u);
				u=par[u];
			}
			else
			{
				path_t.push_back(v);
				v=par[v];
			}
		}
		path_s.push_back(u);
		reverse(path_t.begin(),path_t.end());
		for(int x:path_t)
			path_s.push_back(x);
		for(int tm=0;tm<static_cast<int>(path_s.size());++tm)
		{
			int node=path_s[tm];
			if(w[node]==tm)
				++ans[node];
		}
	}
	for(int i=1;i<=n;++i)
		cout<<ans[i]<<(i==n?'\n':' ');
	return 0;
}
