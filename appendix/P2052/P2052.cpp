// 题目：P2052 [NOI2011] 道路修建
// 思路：删掉一条边后树分成两块，费用 = 边长 × |两块点数之差|
//       用子树大小 sz 表述即 边长 × |sz - (n-sz)|，非递归遍历避免爆栈
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1000005};
constexpr int mxm{2000005};

int H[mxn],to[mxm],nxt[mxm],ew[mxm],ecnt;
int fa[mxn],sz[mxn],ord[mxn];

void addedge(int u,int v,int w)
{
	to[ecnt]=v;
	ew[ecnt]=w;
	nxt[ecnt]=H[u];
	H[u]=ecnt++;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	fill(H,H+mxn,-1);
	for(int i=1;i<n;++i)
	{
		int u,v,w;
		cin>>u>>v>>w;
		--u;--v;
		addedge(u,v,w);
		addedge(v,u,w);
	}
	// 非递归遍历：ord 既是队列也是遍历序
	int tot=1;
	ord[0]=0;
	fa[0]=-1;
	for(int i=0;i<tot;++i)
	{
		int u=ord[i];
		for(int e=H[u];e!=-1;e=nxt[e])
		{
			int v=to[e];
			if(v==fa[u]) continue;
			fa[v]=u;
			ord[tot++]=v;
		}
	}
	// 逆序累加子树大小
	for(int i=1;i<n;++i) sz[i]=1;
	sz[0]=1;
	i64 ans=0;
	for(int i=n-1;i>=1;--i)
	{
		int u=ord[i];
		// 边 (u,fa[u]) 的长度；删掉它后两块大小为 sz[u] 与 n-sz[u]
		int w=0;
		for(int e=H[u];e!=-1;e=nxt[e])
			if(to[e]==fa[u]) w=ew[e];
		ans+=(i64)w*llabs(1LL*sz[u]-(n-sz[u]));
		sz[fa[u]]+=sz[u];
	}
	cout<<ans<<'\n';
	return 0;
}
