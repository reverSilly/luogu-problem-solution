// 题目：P2796 Facer的程序
// 树形计数 DP：f[u]=prod(1+f[v])，答案 sum f[u]，每个连通子集在其最浅点被唯一计数
// BFS 序倒推，避免 1e5 深度递归爆栈
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};
constexpr i64 p{1'000'000'007};

int n,fa[mxn],ord[mxn];
i64 f[mxn],ans;
vector<int>graph[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1,u,v;i<n;++i)
	{
		cin>>u>>v;--u;--v;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}
	// BFS 求出以 0 为根的父节点与层序
	fa[0]=-1;
	int head=0,tail=0;
	ord[tail++]=0;
	while(head<tail)
	{
		int u=ord[head++];
		for(int v:graph[u])
			if(v!=fa[u])
			{
				fa[v]=u;
				ord[tail++]=v;
			}
	}
	for(int i=0;i<n;++i)
		f[i]=1;
	for(int i=n-1;i>=0;--i)	// 倒层序：处理 u 时其子树已全部算完
	{
		int u=ord[i];
		for(int v:graph[u])
			if(v!=fa[u])
				f[u]=f[u]*(f[v]+1)%p;	// 每个孩子子树：不取 / 取含 v 的连通块
		ans=(ans+f[u])%p;
	}
	cout<<ans<<'\n';
	return 0;
}