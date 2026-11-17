// 题目：P2819 图的 m 着色问题
// 逐点回溯计数：每条边恰在其较晚端点被检查一次 ⇒ 局部合法即全局合法
// 度数降序染色：把约束尽早引入，避免菊花图那种"前缀 2^99、答案 2"的爆炸
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int n,k,m,ans,col[mxn],deg[mxn],ord[mxn];
bool g[mxn][mxn];
vector<int>graph[mxn];

void dfs(int d)
{
	if(d==n){++ans;return;}
	int u=ord[d];
	for(int c=0;c<m;++c)
	{
		bool ok=true;
		for(int v:graph[u])
			if(col[v]==c){ok=false;break;}	// 未染色邻居 col=-1，永不等于 c
		if(!ok)
			continue;
		col[u]=c;
		dfs(d+1);
		col[u]=-1;		// 回溯还原成"未着色"，不是 0
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k>>m;
	bool loop=false;
	for(int i=0,u,v;i<k;++i)
	{
		cin>>u>>v;--u;--v;
		if(u==v){loop=true;continue;}	// 自环要求 c(u)!=c(u)，恒无解
		if(!g[u][v])
		{
			g[u][v]=g[v][u]=true;
			graph[u].push_back(v);
			graph[v].push_back(u);
			++deg[u];++deg[v];
		}
	}
	if(loop){cout<<0<<'\n';return 0;}
	fill_n(col,n,-1);	// 全局 int 默认 0 是合法颜色！必须显式置 -1
	iota(ord,ord+n,0);
	sort(ord,ord+n,[](int x,int y){return deg[x]!=deg[y]?deg[x]>deg[y]:x<y;});
	dfs(0);
	cout<<ans<<'\n';
	return 0;
}
