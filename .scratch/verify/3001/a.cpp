// 题目：P3001 [USACO10DEC] Big Macs Around the World G
// 价格连乘 ⇒ 取对数变加法最短路（权可负，Dijkstra 不可用）⇒ Bellman-Ford
// 乘积<1 的环 = 负环；仅当负环从 A 可达且能到 B 时价格无下界，输出 0
// log 域杜绝乘法松弛的上溢/下溢；long double + eps 防零环误报
#include<bits/stdc++.h>
using namespace std;
using f80=long double;

constexpr int mxn{2005},mxm{25005};
constexpr f80 inf{1e18},eps{1e-11};

int n,m,A,B;
f80 V,d[mxn];
struct edge{int u,v;f80 w;}E[mxm];
vector<int>graph[mxn];
bool bad[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>V>>A>>B;--A;--B;
	for(int i=0;i<m;++i)
	{
		f80 e;
		cin>>E[i].u>>E[i].v>>e;--E[i].u;--E[i].v;
		E[i].w=logl(e);
		graph[E[i].u].push_back(E[i].v);
	}
	fill_n(d,n,inf);	// 全局默认 0 不是 INF！d[A]=0 才是唯一合法起点
	d[A]=0;
	for(int it=0;it<n-1;++it)
	{
		bool upd=false;
		for(int i=0;i<m;++i)
		{
			auto[u,v,w]=E[i];
			if(d[u]!=inf&&d[u]+w<d[v])
			{
				d[v]=d[u]+w;
				upd=true;
			}
		}
		if(!upd)
			break;
	}
	// 第 n 轮仍能改进超过 eps ⇒ 可达负环；正向 BFS 圈定受影响点
	queue<int>Q;
	for(int i=0;i<m;++i)
	{
		auto[u,v,w]=E[i];
		if(d[u]!=inf&&d[u]+w<d[v]-eps&&!bad[v])
		{
			bad[v]=true;
			Q.push(v);
		}
	}
	while(!Q.empty())
	{
		int u=Q.front();Q.pop();
		for(int v:graph[u])
			if(!bad[v])
			{
				bad[v]=true;
				Q.push(v);
			}
	}
	if(bad[B]||d[B]==inf)
		cout<<"0\n";
	else
		cout<<fixed<<setprecision(6)<<V*expl(d[B])<<'\n';
	return 0;
}
