//
// 题目：P1653 [USACO05FEB] Ski Lift / 滑雪场缆车
// 暴力基准：直接构建有向图并用 Tarjan 算法求强连通分量缩点
#include<bits/stdc++.h>
using namespace std;

constexpr int mxv{250005};
constexpr int dr[4]{-1,1,0,0},dc[4]{0,0,-1,1};

int W,L,h[505][505];
vector<int>graph[mxv];
int dfn[mxv],low[mxv],scc[mxv],timer=0,scnt=0;
bool in_stk[mxv],has_in[mxv],has_out[mxv];
vector<int>stk;

void tarjan(int u)
{
	dfn[u]=low[u]=++timer;
	stk.push_back(u);
	in_stk[u]=true;
	for(int v:graph[u])
	{
		if(!dfn[v])
		{
			tarjan(v);
			low[u]=min(low[u],low[v]);
		}
		else if(in_stk[v])
			low[u]=min(low[u],dfn[v]);
	}
	if(low[u]==dfn[u])
	{
		while(true)
		{
			int x=stk.back();stk.pop_back();
			in_stk[x]=false;
			scc[x]=scnt;
			if(x==u)
				break;
		}
		++scnt;
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>W>>L;
	for(int r=0;r<L;++r)
		for(int c=0;c<W;++c)
			cin>>h[r][c];

	int n=L*W;
	for(int r=0;r<L;++r)
		for(int c=0;c<W;++c)
		{
			int u=r*W+c;
			for(int d=0;d<4;++d)
			{
				int vr=r+dr[d],vc=c+dc[d];
				if(vr>=0&&vr<L&&vc>=0&&vc<W&&h[r][c]>=h[vr][vc])
					graph[u].push_back(vr*W+vc);
			}
		}

	for(int i=0;i<n;++i)
		if(!dfn[i])
			tarjan(i);

	if(scnt<=1)
	{
		cout<<0<<'\n';
		return 0;
	}

	for(int u=0;u<n;++u)
		for(int v:graph[u])
			if(scc[u]!=scc[v])
			{
				has_out[scc[u]]=true;
				has_in[scc[v]]=true;
			}

	int src=0,snk=0;
	for(int i=0;i<scnt;++i)
	{
		if(!has_in[i])
			++src;
		if(!has_out[i])
			++snk;
	}
	cout<<max(src,snk)<<'\n';
	return 0;
}
