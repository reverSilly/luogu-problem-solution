// 题目：P2872 [USACO07DEC] Building Roads S
// 思路：已有边免费 ⇒ 并查集先缩点；再对完全图跑 Kruskal。
//       边权存平方距离（i64，防溢出），排序等价于按距离排（sqrt 单调）；
//       只有选中的 <=n-1 条边才开方，浮点误差源最少。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005},mxm{500005};

int n,m,x[mxn],y[mxn],f[mxn],cnt;
struct edge{int u,v;i64 w;}E[mxm];
double ans;

int getrt(int u)
{
	if(f[u]==u)
		return u;
	return f[u]=getrt(f[u]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>x[i]>>y[i];
	iota(f,f+n,0);
	for(int i=0,u,v;i<m;++i)
	{
		cin>>u>>v;--u;--v;
		f[getrt(u)]=getrt(v);	// 已有边免费，先缩点
	}
	for(int i=0;i<n;++i)
		for(int j=i+1;j<n;++j)
		{
			i64 dx=x[i]-x[j],dy=y[i]-y[j];
			E[cnt++]={i,j,dx*dx+dy*dy};	// 平方距离最大 2e12，int 会炸
		}
	sort(E,E+cnt,[](const edge&a,const edge&b){return a.w<b.w;});
	for(int i=0;i<cnt;++i)
	{
		auto[u,v,w]=E[i];
		u=getrt(u);v=getrt(v);
		if(u==v)
			continue;
		f[u]=v;
		ans+=sqrt((double)w);	// 只对选中边开方
	}
	cout<<fixed<<setprecision(2)<<ans<<'\n';
	return 0;
}
