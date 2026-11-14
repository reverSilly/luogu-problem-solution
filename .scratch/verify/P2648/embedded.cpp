// 题目：P2648 赚钱
// 思路：每到一城赚 D ⇒ 路径边权 D、航线边权 D-T，旅行=途径、收益=边权和。
//       超级源 0 向每城连权 D 的边（起点任意），答案取各城最长路最大（终点任意）。
//       正环 ⇔ 无限赚（orz）；Bellman-Ford 最长路，第 V 轮仍松弛 ⇔ 正环。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{305},mxm{1205},inf{1145141919};

int D,P,C,F,dis[mxn];
struct edge{int u,v,w;}E[mxm];
int ecnt;

void addedge(int u,int v,int w){E[ecnt++]={u,v,w};}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>D>>P>>C>>F;
	int V=C+1;					// 0 号超级源 + C 座城市
	for(int i=1;i<=C;++i)
		addedge(0,i,D);			// 从任意城市出发，先赚 D
	for(int i=0,a,b;i<P;++i)
	{
		cin>>a>>b;
		addedge(a,b,D);			// 免费路径：走到下一城再赚 D
	}
	for(int i=0,j,k,t;i<F;++i)
	{
		cin>>j>>k>>t;
		addedge(j,k,D-t);		// 航线：赚 D 但付机票 t
	}
	fill_n(dis,V,-inf);			// 不可达必须显式 -inf，全局默认 0 是陷阱
	dis[0]=0;
	bool orz=false;
	for(int it=0;it<V;++it)		// Bellman-Ford 最长路，共 V 轮
	{
		bool chg=false;
		for(int e=0;e<ecnt;++e)
		{
			auto[u,v,w]=E[e];
			if(dis[u]>-inf&&dis[u]+w>dis[v])
			{
				dis[v]=dis[u]+w;
				chg=true;
			}
		}
		if(!chg)
			break;				// 提前收敛 ⇒ 无正环
		if(it==V-1)
			orz=true;			// 第 V 轮仍能松弛 ⇒ 存在正环
	}
	if(orz)
		cout<<"orz\n";
	else
	{
		int ans=0;
		for(int i=1;i<=C;++i)
			ans=max(ans,dis[i]);
		cout<<ans<<'\n';
	}
	return 0;
}