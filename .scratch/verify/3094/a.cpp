// 题目：P3094 [USACO13DEC] Vacation Planning S
// N<=200 ⇒ Floyd 全源最短路；请求 (a,b) 必须经过枢纽 ⇒ 答案=min_h dis[a][h]+dis[h][b]。
// 正权 ⇒ 最优游走可拆成两段最短路；拼接合法因为允许重复经过农场。
// 总费用可达 1e4*2e8=2e12 ⇒ i64；inf 用 i64 哨兵，避免 int INF+INF 溢出。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{205};
constexpr i64 inf{1145141919810};

int n,m,k,q,cnt;
i64 dis[mxn][mxn],ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>k>>q;
	fill_n(&dis[0][0],mxn*mxn,inf);	// 全局默认 0 不是 INF！
	for(int i=0;i<n;++i)
		dis[i][i]=0;
	for(int i=0,u,v,w;i<m;++i)
	{
		cin>>u>>v>>w;--u;--v;
		dis[u][v]=min(dis[u][v],(i64)w);	// 重边取最小
	}
	for(int t=0;t<n;++t)
		for(int i=0;i<n;++i)
			for(int j=0;j<n;++j)
				dis[i][j]=min(dis[i][j],dis[i][t]+dis[t][j]);
	for(int i=0,a,b;i<q;++i)
	{
		cin>>a>>b;--a;--b;
		i64 best=inf;
		for(int h=0;h<k;++h)
			best=min(best,dis[a][h]+dis[h][b]);
		if(best<inf)
			++cnt,ans+=best;
	}
	cout<<cnt<<'\n'<<ans<<'\n';
	return 0;
}
