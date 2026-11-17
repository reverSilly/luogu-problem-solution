// 题目：P2820 局域网
// 删边权和最大 = tot - 保留边权和最小；"无环+连通性不变" ⟺ 保留集是生成森林 ⇒ Kruskal
// k 无上界而 n<=100 ⇒ 自环必删、平行边只留最便宜的一条，先把边集压到 <=4950 条
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{105},inf{1145141919};

int n,k,cnt,mn[mxn][mxn],f[mxn];
i64 tot,kept;
struct edge{int u,v,w;}E[mxn*mxn];

int getrt(int x)
{
	if(f[x]==x)
		return x;
	return f[x]=getrt(f[x]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	fill_n(&mn[0][0],mxn*mxn,inf);	// 默认 0 配 min 更新会丢光所有边！
	cin>>n>>k;
	for(int i=0,u,v,c;i<k;++i)
	{
		cin>>u>>v>>c;--u;--v;
		tot+=c;
		if(u!=v&&c<mn[u][v])
			mn[u][v]=mn[v][u]=c;	// 自环不进矩阵：必删，权已计入 tot
	}
	for(int u=0;u<n;++u)
		for(int v=u+1;v<n;++v)
			if(mn[u][v]<inf)
				E[cnt++]={u,v,mn[u][v]};
	sort(E,E+cnt,[](edge a,edge b){return a.w<b.w;});
	iota(f,f+n,0);
	for(int i=0;i<cnt;++i)
	{
		int a=getrt(E[i].u),b=getrt(E[i].v);
		if(a==b)
			continue;		// 两端已连通 ⇒ 这条边成环，被删除
		f[a]=b;
		kept+=E[i].w;
	}
	cout<<tot-kept<<'\n';	// 不连通时是生成森林，Kruskal 天然支持
	return 0;
}
