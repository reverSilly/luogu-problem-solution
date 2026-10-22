#include<bits/stdc++.h>
using namespace std;

using i64=long long;

//暴力：Kruskal。重边按“最后读入的边权生效”去重（与官方数据处理一致）
constexpr int mxn{2405};
constexpr int mxm{400005};

struct edge{int u,v,w;};

edge E[mxm];
int fa[mxn];

int getrt(int x)
{
	return fa[x]==x?x:fa[x]=getrt(fa[x]);
}

int main()
{
	cin.tie(nullptr);
	ios::sync_with_stdio(false);
	int n,m;
	cin>>n>>m;
	map<pair<int,int>,int>mp;
	for(int i=0;i<m;++i)
	{
		int a,b,c;
		cin>>a>>b>>c;
		if(a>b)swap(a,b);
		mp[{a,b}]=c;//覆盖，保留最新边权
	}
	int cnt=0;
	for(auto&kv:mp)
	{
		E[cnt]={kv.first.first,kv.first.second,kv.second};
		++cnt;
	}
	sort(E,E+cnt,[](edge a,edge b){return a.w<b.w;});
	for(int i=0;i<n+1;++i)fa[i]=i;
	i64 ans=0;
	int used=0;
	for(int i=0;i<cnt;++i)
	{
		int u=getrt(E[i].u),v=getrt(E[i].v);
		if(u==v)continue;
		fa[u]=v;
		ans+=E[i].w;
		if(++used==n)break;
	}
	cout<<ans<<'\n';
	return 0;
}
