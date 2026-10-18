//
// 题目：P1525 [NOIP 2010 提高组] 关押罪犯
// https://www.luogu.com.cn/problem/P1525
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{20005};         // N<=20000 (+5 松弛)
constexpr int mxm{100005};        // M<=100000 (+5 松弛)

int n,m;
int f[mxn*2];                     // 扩展域并查集: [0,n) 为同狱域, [n,2n) 为敌对异狱域

struct Edge
{
	int u,v,w;
	friend bool operator<(Edge a,Edge b){return a.w>b.w;}
}E[mxm];

int getrt(int x){return f[x]==x?x:f[x]=getrt(f[x]);}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>m))
		return 0;
	for(int i=0;i<m;++i)
	{
		cin>>E[i].u>>E[i].v>>E[i].w;
		--E[i].u;--E[i].v;
	}
	sort(E,E+m);
	iota(f,f+2*n,0);

	for(int i=0;i<m;++i)
	{
		auto[u,v,w]=E[i];
		if(getrt(u)==getrt(v))
		{
			cout<<w<<'\n';        // 首条无法被分入不同监狱的边(奇环闭合处)即为最小可能的最大冲突值
			return 0;
		}
		f[getrt(u)]=getrt(v+n);
		f[getrt(v)]=getrt(u+n);
	}
	cout<<0<<'\n';
	return 0;
}
