// 题目：P2185 公路通行税
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005};
constexpr int mxm{50005};

int head[mxn],nxt[mxm],to[mxm],ecnt;
int dist_[mxn],que[mxn];

void adde(int u,int v)
{
	++ecnt;
	to[ecnt]=v;
	nxt[ecnt]=head[u];
	head[u]=ecnt;
}

void bfs(int s,int n)
{
	for(int i=1;i<=n;++i)
		dist_[i]=-1;
	int l=0,r=0;
	que[r++]=s;
	dist_[s]=0;
	while(l<r)
	{
		int u=que[l++];
		for(int e=head[u];e;e=nxt[e])
		{
			int v=to[e];
			if(dist_[v]!=-1)
				continue;
			dist_[v]=dist_[u]+1;
			que[r++]=v;
		}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n,m;
	while(cin>>n>>m)
	{
		if(!n&&!m)
			break;
		for(int i=1;i<=n;++i)
			head[i]=0;
		ecnt=0;
		for(int i=1;i<=m;++i)
		{
			int u,v;
			cin>>u>>v;
			adde(u,v),adde(v,u);
		}
		// 图连通，从每个点出发 BFS，最远距离的最大值即为直径（边数）
		int diam=0;
		for(int s=1;s<=n;++s)
		{
			bfs(s,n);
			for(int i=1;i<=n;++i)
				diam=max(diam,dist_[i]);
		}
		// 印花价值 = 直径条公路 × 100 次旅行 × 每次 1 PC
		cout<<(i64)diam*100<<'\n';
	}
	return 0;
}
