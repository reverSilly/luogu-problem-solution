// 题目：P2932 [USACO09JAN] Earthquake Damage G
// 报告点 r 未受损却回不到谷仓 ⇒ r 的每个邻点要么受损要么也回不去，总之都计入答案
// 把全体报告点的邻点标记为受损(bad)，从谷仓 BFS，答案 = P - 可达数
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{30005};

int p,c,n;
vector<int>graph[mxn];
bool bad[mxn],vis[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>p>>c>>n;
	for(int i=0,u,v;i<c;++i)
	{
		cin>>u>>v;--u;--v;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}
	for(int i=0,r;i<n;++i)
	{
		cin>>r;--r;
		for(int v:graph[r])
			bad[v]=true;	// 报告点自身未受损，但其邻点必须被封锁
	}
	queue<int>Q;
	int reached=0;
	if(!bad[0])	// 谷仓若与报告点相邻则必受损，可达数为 0
	{
		vis[0]=true;
		Q.push(0);
	}
	while(!Q.empty())
	{
		int u=Q.front();Q.pop();
		++reached;
		for(int v:graph[u])
			if(!bad[v]&&!vis[v])
			{
				vis[v]=true;
				Q.push(v);
			}
	}
	cout<<p-reached<<'\n';
	return 0;
}
