#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{20005};

int n,m,p,q;
//v[]：并查集式的手写邻接表（BFS 求连通块）
int head[mxn],to[mxn*2],nxt[mxn*2],ecnt;

bool vis[mxn];

int mp(int x)
{
	return x>0?x:n-x;
}

void add(int u,int v)
{
	++ecnt;
	to[ecnt]=v;
	nxt[ecnt]=head[u];
	head[u]=ecnt;
}

//BFS 求从 s 出发能到达的人数（含 s 本身）
int bfs(int s)
{
	for(int i=0;i<=n+m;++i)
		vis[i]=false;
	int qh=0,qt=0;
	int que[mxn];
	que[qt++]=s;
	vis[s]=true;
	int cnt=0;
	while(qh<qt)
	{
		int u=que[qh++];
		++cnt;
		for(int i=head[u];i;i=nxt[i])
		{
			int v=to[i];
			if(!vis[v])
			{
				vis[v]=true;
				que[qt++]=v;
			}
		}
	}
	return cnt;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>p>>q;
	for(int i=1;i<=p;++i)
	{
		int u,v;
		cin>>u>>v;
		u=mp(u);
		v=mp(v);
		add(u,v);
		add(v,u);
	}
	for(int i=1;i<=q;++i)
	{
		int u,v;
		cin>>u>>v;
		u=mp(u);
		v=mp(v);
		add(u,v);
		add(v,u);
	}
	int a=bfs(1);
	int b=bfs(n+1);
	cout<<min(a,b)<<'\n';
	return 0;
}
