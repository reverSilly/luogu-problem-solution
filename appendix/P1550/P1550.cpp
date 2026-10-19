// 题目：P1550 [USACO08OCT] Watering Hole G
// https://www.luogu.com.cn/problem/P1550
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{305};           // n<=300 (+5 松弛)
constexpr int mxm{50005};         // n + n(n-1)/2 <= 45150 (+5 松弛)

int n;
int W[mxn];                       // W[i]=在 i 号田挖井的花费
int P[mxn][mxn];                  // P[i][j]=连接 i,j 的花费
struct{int u,v,w;}E[mxm];         // 边表;源点编号 n
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

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>W[i];
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>P[i][j];

	int m=0;
	for(int i=0;i<n;++i)
		E[m++]={i,n,W[i]};                        // 挖井: 虚源点 n <-> 田 i,权 W[i]
	for(int i=0;i<n;++i)
		for(int j=i+1;j<n;++j)
			E[m++]={i,j,P[i][j]};                 // 修水道: 田 i <-> 田 j

	sort(E,E+m,[](auto a,auto b){return a.w<b.w;});
	iota(f,f+n+1,0);                              // n+1 个节点(含源点)
	i64 ans=0;
	int cnt=0;
	for(int i=0;i<m&&cnt<n;++i)                   // n+1 个点连通需 n 条边
	{
		int ru=getrt(E[i].u),rv=getrt(E[i].v);
		if(ru!=rv)
		{
			f[ru]=rv;
			ans+=E[i].w;
			++cnt;
		}
	}
	cout<<ans<<'\n';
	return 0;
}
