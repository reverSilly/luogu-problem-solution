// 题目：P2194 HXY烧情侣
// 思路：一条回路（闭合行走）完全落在某个强连通分量内
//       每个分量至少要烧一次，最优费用 = 分量内最小点权
//       方案数 = 分量内最小点权的出现次数，各分量相乘
//       n=1e5 递归 Tarjan 容易爆栈，这里写非递归 Tarjan
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};
constexpr int mxe{300005};
constexpr i64 p{1000000007};

int n,m,w[mxn];
int head[mxn],nxt[mxe],to[mxe],ecnt=0;

int dfn[mxn],low[mxn],comp[mxn],idx[mxn];
int stk[mxn],top=0;          // Tarjan 栈
int dfs[mxn],dftop=0;        // DFS 调用栈
bool instk[mxn];
int timer=0,cc=0;

void tarjan(int s)
{
	dfn[s]=low[s]=++timer;
	idx[s]=head[s];
	instk[s]=1;
	stk[top++]=s;
	dfs[dftop++]=s;
	while(dftop)
	{
		int u=dfs[dftop-1];
		if(idx[u])
		{
			int e=idx[u];idx[u]=nxt[e];
			int v=to[e];
			if(!dfn[v])
			{
				dfn[v]=low[v]=++timer;
				idx[v]=head[v];
				instk[v]=1;
				stk[top++]=v;
				dfs[dftop++]=v;
			}
			else if(instk[v])low[u]=min(low[u],dfn[v]);
		}
		else
		{
			--dftop;
			if(low[u]==dfn[u])
			{
				++cc;
				int x;
				do
				{
					x=stk[--top];
					comp[x]=cc;
					instk[x]=0;
				}while(x!=u);
			}
			if(dftop)
			{
				int fa=dfs[dftop-1];
				low[fa]=min(low[fa],low[u]);
			}
		}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1;i<=n;++i)cin>>w[i];
	cin>>m;
	for(int i=1;i<=m;++i)
	{
		int x,y;cin>>x>>y;
		++ecnt;to[ecnt]=y;nxt[ecnt]=head[x];head[x]=ecnt;
	}
	for(int i=1;i<=n;++i)
		if(!dfn[i])tarjan(i);

	int mn[mxn],cnt[mxn];
	for(int i=1;i<=cc;++i)mn[i]=1145141919,cnt[i]=0;
	for(int i=1;i<=n;++i)
	{
		int c=comp[i];
		if(w[i]<mn[c])
		{
			mn[c]=w[i];
			cnt[c]=1;
		}
		else if(w[i]==mn[c])++cnt[c];
	}
	i64 cost=0,ways=1;
	for(int i=1;i<=cc;++i)
	{
		cost+=mn[i];
		ways=ways*cnt[i]%p;
	}
	cout<<cost<<' '<<ways<<'\n';
	return 0;
}
