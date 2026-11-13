// 题目：P2420 让我们异或吧
// 思路：d[u]=根到 u 的边权异或和；path(u,v) 的异或 = d[u]^d[v]
//       公共根->lca 段因 x^x=0 自动抵消。BFS 预处理 d，询问 O(1)
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

int n,m,d[mxn];
vector<pair<int,int>>graph[mxn];
int Q[mxn];
bool vis[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1,u,v,w;i<n;++i)
	{
		cin>>u>>v>>w;--u;--v;
		graph[u].push_back({v,w});
		graph[v].push_back({u,w});
	}
	// BFS from root 0, d[u]=xor of edge weights on root->u path
	int hd=0,tl=0;
	Q[tl++]=0;vis[0]=1;d[0]=0;
	while(hd<tl)
	{
		int u=Q[hd++];
		for(auto[v,w]:graph[u])
			if(!vis[v])
			{
				vis[v]=1;
				d[v]=d[u]^w;
				Q[tl++]=v;
			}
	}
	cin>>m;
	for(int i=0,u,v;i<m;++i)
	{
		cin>>u>>v;--u;--v;
		cout<<(d[u]^d[v])<<'\n';
	}
	return 0;
}
