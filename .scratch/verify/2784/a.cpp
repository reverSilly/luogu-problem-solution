// 题目：P2784 化学1（chem1）- 化学合成
// 路线产量 = 转化率之积；取 -ln 后变成边权全正的最短路（0<C<1 ⇒ -ln C>0）。
// Dijkstra 求 S→T 最小权值和，答案 expl(-dis[T])；不可达输出 orz。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};
constexpr long double inf{1e18};

int n,m,S,T;
vector<pair<int,long double>>graph[mxn];
long double dis[mxn];
bool vis[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>S>>T;--S;--T;
	for(int i=0,u,v;i<m;++i)
	{
		long double c;
		cin>>u>>v>>c;--u;--v;
		graph[u].push_back({v,-logl(c)});	// 乘法产量 ⇒ 加法 -ln C，恒正
	}
	fill_n(dis,n,inf);	// 全局数组默认 0，不可达必须显式填 INF
	dis[S]=0;
	priority_queue<pair<long double,int>,vector<pair<long double,int>>,greater<>>Q;
	Q.push({0,S});
	while(!Q.empty())
	{
		auto[d,u]=Q.top();Q.pop();
		if(vis[u])
			continue;
		vis[u]=true;
		for(auto[v,w]:graph[u])
			if(dis[u]+w<dis[v])
			{
				dis[v]=dis[u]+w;
				Q.push({dis[v],v});
			}
	}
	if(dis[T]>=inf)
		cout<<"orz\n";
	else
		cout<<fixed<<setprecision(4)<<expl(-dis[T])<<'\n';
	return 0;
}
