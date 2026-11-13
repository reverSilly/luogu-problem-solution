#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005};

int n,m;
vector<int>graph[mxn],rg[mxn];
bool can[mxn],ok[mxn];
int dis[mxn],Q[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0,u,v;i<m;++i)
	{
		cin>>u>>v;--u;--v;
		graph[u].push_back(v);
		rg[v].push_back(u);
	}
	int s,t;
	cin>>s>>t;--s;--t;

	int hd=0,tl=0;
	can[t]=true;
	Q[tl++]=t;
	while(hd<tl)
	{
		int u=Q[hd++];
		for(int v:rg[u])
			if(!can[v])
			{
				can[v]=true;
				Q[tl++]=v;
			}
	}
	for(int u=0;u<n;++u)
	{
		ok[u]=true;
		for(int v:graph[u])
			if(!can[v])
			{
				ok[u]=false;
				break;
			}
	}
	if(!ok[s]||!ok[t])
	{
		cout<<-1<<'\n';
		return 0;
	}
	fill_n(dis,n,-1);
	hd=tl=0;
	dis[s]=0;
	Q[tl++]=s;
	while(hd<tl)
	{
		int u=Q[hd++];
		if(u==t)
			break;
		for(int v:graph[u])
			if(ok[v]&&dis[v]==-1)
			{
				dis[v]=dis[u]+1;
				Q[tl++]=v;
			}
	}
	cout<<dis[t]<<'\n';
	return 0;
}
