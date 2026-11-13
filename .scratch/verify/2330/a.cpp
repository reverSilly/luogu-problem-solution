#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{305},mxm{8005};

int n,m;
struct edge{int u,v,w;}E[mxm];
int f[mxn];

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
	sort(E,E+m,[](auto a,auto b){return a.w<b.w;});
	for(int i=0;i<n;++i)
		f[i]=i;
	int cnt=0,ans=0;
	for(int i=0;i<m&&cnt<n-1;++i)
	{
		int a=getrt(E[i].u),b=getrt(E[i].v);
		if(a==b)
			continue;
		f[a]=b;
		++cnt;
		ans=E[i].w;
	}
	cout<<n-1<<' '<<ans<<'\n';
	return 0;
}
