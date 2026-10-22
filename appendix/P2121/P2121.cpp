#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

struct edge{int u,v,w;};

int f[mxn];
edge E[mxn];

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

	int n,m,K;
	cin>>n>>m>>K;
	for(int i=0;i<m;++i)
	{
		cin>>E[i].u>>E[i].v>>E[i].w;
		--E[i].u;--E[i].v;
	}
	//按美丽度从大到小排序，贪心取不成环的前 K 条
	sort(E,E+m,[](edge a,edge b){return a.w>b.w;});

	iota(f,f+n,0);
	i64 ans=0;
	int cnt=0;
	for(int i=0;i<m&&cnt<K;++i)
	{
		int u=getrt(E[i].u),v=getrt(E[i].v);
		if(u==v)
			continue;//这条边会成环，丢弃
		f[u]=v;
		ans+=E[i].w;
		++cnt;
	}
	cout<<ans<<'\n';
	return 0;
}
