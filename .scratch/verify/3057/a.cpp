// 题目：P3057 [USACO12NOV] Distant Pastures S
// 格子=点，相邻边权 A(同草)/B(异草)；答案=图直径。
// V<=900：Floyd 7.3e8 危险，多源 Dijkstra ~3e7 稳过。B<A 时最短路会绕路，不能套曼哈顿公式。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{905};
constexpr i64 inf{1145141919810};
constexpr int dr[4]{-1,1,0,0},dc[4]{0,0,-1,1};

int n,A,B;
char g[35][35];
i64 dis[mxn],ans;
bool vis[mxn];

void dij(int s)
{
	fill_n(dis,n*n,inf);	// 每轮必须重填 INF，全局 0 不是无穷
	fill_n(vis,n*n,false);
	priority_queue<pair<i64,int>,vector<pair<i64,int>>,greater<>>Q;
	dis[s]=0;
	Q.emplace(0,s);
	while(!Q.empty())
	{
		auto[d,u]=Q.top();Q.pop();
		if(vis[u])
			continue;	// 惰性删除：过期条目
		vis[u]=true;
		int r=u/n,c=u%n;
		for(int k=0;k<4;++k)
		{
			int nr=r+dr[k],nc=c+dc[k];
			if(nr<0||nr>=n||nc<0||nc>=n)
				continue;
			int v=nr*n+nc;
			i64 w=g[nr][nc]==g[r][c]?A:B;
			if(dis[u]+w<dis[v])
				dis[v]=dis[u]+w,Q.emplace(dis[v],v);
		}
	}
	for(int i=0;i<n*n;++i)
		ans=max(ans,dis[i]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>A>>B;
	for(int i=0;i<n;++i)
		cin>>g[i];
	for(int s=0;s<n*n;++s)
		dij(s);
	cout<<ans<<'\n';
	return 0;
}
