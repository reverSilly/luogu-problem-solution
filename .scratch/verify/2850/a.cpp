// 题目：P2850 [USACO06DEC] Wormholes G
// 思路：路径=正权双向边，虫洞=负权单向边；「回到出发之前」⟺ 存在负环（严格负）。
//       起点自由 ⇒ 虚拟超级源点 ⇒ dis 全 0 起步的 Bellman-Ford；
//       第 n 轮仍能松弛 ⟺ 有负环；某轮无松弛即可提前判 NO。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{505},mxm{5205};	// 2*2500+200+5

struct edge{int u,v,w;}E[mxm];
int n,m,w,ec,dis[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int F;
	cin>>F;
	while(F--)
	{
		cin>>n>>m>>w;
		ec=0;
		for(int i=0,u,v,t;i<m;++i)
		{
			cin>>u>>v>>t;--u;--v;
			E[ec++]={u,v,t};	// 双向路径拆成两条正权有向边，重边照存
			E[ec++]={v,u,t};
		}
		for(int i=0,u,v,t;i<w;++i)
		{
			cin>>u>>v>>t;--u;--v;
			E[ec++]={u,v,-t};	// 虫洞：单向负权；S==E 的负自环也能被自动判出
		}
		fill_n(dis,n,0);	// 超级源点向所有点连 0 边 ⟺ 全 0 初始化（这里 0 不是"未访问"！）
		int it=0;
		for(;it<n;++it)
		{
			bool chg=false;
			for(int i=0;i<ec;++i)
				if(dis[E[i].v]>dis[E[i].u]+E[i].w)
				{
					dis[E[i].v]=dis[E[i].u]+E[i].w;
					chg=true;
				}
			if(!chg)
				break;	// 三角不等式全部成立 ⇒ 无负环
		}
		cout<<(it==n?"YES\n":"NO\n");	// 跑满 n 轮仍在松弛 ⇒ 负环
	}
	return 0;
}
