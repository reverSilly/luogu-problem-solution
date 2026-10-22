// 题目：P2097 资料分发 1 —— 暴力：BFS 求连通块个数
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{205};

int n,m;
int head[mxn],nxt[mxn*4],to[mxn*4],ecnt;
bool vis[mxn];

void bfs(int s)
{
	int q[mxn*4],l=0,r=0;
	q[r++]=s;
	vis[s]=true;
	while(l<r)
	{
		int u=q[l++];
		for(int e=head[u];e!=-1;e=nxt[e])
		{
			int v=to[e];
			if(vis[v]) continue;
			vis[v]=true;
			q[r++]=v;
		}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	fill_n(head,mxn,-1);
	for(int i=0,u,v;i<m;++i)
	{
		cin>>u>>v;
		to[ecnt]=v; nxt[ecnt]=head[u]; head[u]=ecnt++; ++ecnt;
		to[ecnt]=u; nxt[ecnt]=head[v]; head[v]=ecnt++; ++ecnt;
	}
	int ans=0;
	for(int i=1;i<=n;++i)
		if(!vis[i])
		{
			++ans;
			bfs(i);
		}
	cout<<ans<<'\n';
	return 0;
}
