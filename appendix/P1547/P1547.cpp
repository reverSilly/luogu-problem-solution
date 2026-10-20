// 题目：P1547 [USACO05MAR] Out of Hay S
// https://www.luogu.com.cn/problem/P1547
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2005};          // N<=2000 (+5 松弛)
constexpr int mxm{10005};         // M<=10000 (+5 松弛)

int n,m;
struct{int u,v,w;}E[mxm];         // 边表(读入即 0-based 端点)
int f[mxn];                       // 并查集父指针

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

	cin>>n>>m;
	for(int i=0;i<m;++i)
	{
		cin>>E[i].u>>E[i].v>>E[i].w;
		--E[i].u;--E[i].v;
	}
	sort(E,E+m,[](auto a,auto b){return a.w<b.w;});   // 边权升序
	iota(f,f+n,0);
	int ans=0,cnt=0;
	for(int i=0;i<m&&cnt<n-1;++i)                     // 只需 n-1 条边
	{
		int ru=getrt(E[i].u),rv=getrt(E[i].v);
		if(ru!=rv)
		{
			f[ru]=rv;
			ans=E[i].w;                               // 最后加入的边权即 MST 最大边
			++cnt;
		}
	}
	cout<<ans<<'\n';
	return 0;
}
