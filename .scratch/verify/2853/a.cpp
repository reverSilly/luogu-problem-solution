// 题目：P2853 [USACO06DEC] Cow Picnic S
// 思路：答案 = K 个单源可达集的交集大小。
//       逐源 BFS，每头牛给可达点 cnt++；cnt[v]==K 即在交集里。
//       反图多源 BFS 求的是并集，不能用！
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int k,n,m,c[105],cnt[mxn],vis[mxn],q[mxn];
vector<int>graph[mxn];

void bfs(int s)
{
	fill_n(vis,n,0);	// vis 每次重置；cnt 全程累计
	int hd=0,tl=0;
	vis[s]=1;++cnt[s];q[tl++]=s;	// 自己所在牧场也算可达（走 0 步）
	while(hd<tl)
	{
		int u=q[hd++];
		for(int v:graph[u])
			if(!vis[v])
			{
				vis[v]=1;++cnt[v];	// 入队时置位 ⇒ 每点恰计一次
				q[tl++]=v;
			}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>k>>n>>m;	// 注意顺序是 K,N,M，不是 N,M,K
	for(int i=0;i<k;++i)
	{
		cin>>c[i];--c[i];
	}
	for(int i=0,u,v;i<m;++i)
	{
		cin>>u>>v;--u;--v;
		graph[u].push_back(v);	// 有向边，只加 u→v
	}
	for(int i=0;i<k;++i)
		bfs(c[i]);	// 重复起点照跑：两头牛各贡献一次，天然正确
	int ans=0;
	for(int i=0;i<n;++i)
		if(cnt[i]==k)
			++ans;
	cout<<ans<<'\n';
	return 0;
}
