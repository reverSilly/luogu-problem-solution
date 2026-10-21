//
// P1607 暴力（对拍用）：时间展开图上的最小费用最大流
// 把每头牛看作 1 单位流：链边 i->i+1 容量 C，组边 S->E 容量 M 费用 -1
// 从 1 号站推 C 单位流到 N 号站，最小费用的相反数即最大载客数
#include<bits/stdc++.h>
using namespace std;

constexpr int V{105},E{10005};
constexpr int inf{1000000000};

struct{int v,w,c,nx;}ed[E];
int H[V],cnt;
void ae(int u,int v,int w,int c)
{
	ed[cnt]={v,w,c,H[u]};H[u]=cnt++;
	ed[cnt]={u,0,-c,H[v]};H[v]=cnt++;
}

int dis[V],inq[V],pre[V],pe[V];

int main()
{
	int K,N,C;
	if(!(cin>>K>>N>>C))
		return 0;
	fill_n(H,N,-1);
	for(int i=0;i+1<N;++i)
		ae(i,i+1,C,0);
	for(int i=0,s,e,m;i<K;++i)
	{
		cin>>s>>e>>m;
		--s;--e;
		if(s<e)
			ae(s,e,m,-1);
	}
	int S=0,T=N-1,flow=0,cost=0;
	while(flow<C)
	{
		fill_n(dis,N,inf);
		fill_n(inq,N,0);
		queue<int>q;
		dis[S]=0;inq[S]=1;q.push(S);
		while(!q.empty())
		{
			int u=q.front();q.pop();inq[u]=0;
			for(int i=H[u];i!=-1;i=ed[i].nx)
			{
				int v=ed[i].v;
				if(ed[i].w>0&&dis[u]+ed[i].c<dis[v])
				{
					dis[v]=dis[u]+ed[i].c;
					pre[v]=u;pe[v]=i;
					if(!inq[v]){inq[v]=1;q.push(v);}
				}
			}
		}
		if(dis[T]>=inf)
			break;
		int add=C-flow;
		for(int v=T;v!=S;v=pre[v])
			add=min(add,ed[pe[v]].w);
		for(int v=T;v!=S;v=pre[v])
		{
			ed[pe[v]].w-=add;
			ed[pe[v]^1].w+=add;
		}
		flow+=add;
		cost+=add*dis[T];
	}
	cout<<-cost<<'\n';
	return 0;
}
