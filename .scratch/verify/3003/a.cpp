// 题目：P3003 [USACO10DEC] Apple Delivery S
// 答案 = d(A1,A2) + min(d(B,A1), d(B,A2))，从 A1、A2 各跑一次 Dijkstra 即可
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005},mxm{200005};
constexpr i64 inf{1145141919810};

int C,P,b,a1,a2,cnt,H[mxn];
struct{int v,w,nx;}E[mxm*2];
i64 d1[mxn],d2[mxn];
bool vis[mxn];

void addedge(int u,int v,int w)
{
	E[cnt]={v,w,H[u]};H[u]=cnt++;
}

void dij(int s,i64*dis)
{
	fill_n(dis,P,inf);
	fill_n(vis,P,false);
	priority_queue<pair<i64,int>,vector<pair<i64,int>>,greater<>>Q;
	dis[s]=0;Q.emplace(0,s);
	while(!Q.empty())
	{
		auto[d,u]=Q.top();Q.pop();
		if(vis[u])
			continue;
		vis[u]=true;
		for(int i=H[u];i!=-1;i=E[i].nx)
		{
			int v=E[i].v;
			if(dis[v]>d+E[i].w)
			{
				dis[v]=d+E[i].w;
				Q.emplace(dis[v],v);
			}
		}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>C>>P>>b>>a1>>a2;--b;--a1;--a2;	// 注意输入顺序是 C 在前 P 在后
	fill_n(H,P,-1);	// H 必须填 -1：0 是合法边编号，全局默认 0 会静默出错
	for(int i=0,u,v,w;i<C;++i)
	{
		cin>>u>>v>>w;--u;--v;
		addedge(u,v,w);
		addedge(v,u,w);
	}
	dij(a1,d1);
	dij(a2,d2);
	cout<<d1[a2]+min(d1[b],d2[b])<<'\n';
	return 0;
}
