// 题目：P2097 资料分发 1
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

int n,m;
int fa[mxn];

int getrt(int x)
{
	if(fa[x]==x)
		return x;
	return fa[x]=getrt(fa[x]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		fa[i]=i;
	int ans=n;
	for(int i=0,u,v;i<m;++i)
	{
		cin>>u>>v;--u;--v;
		int a=getrt(u),b=getrt(v);
		if(a!=b)
		{
			fa[a]=b;
			--ans;
		}
	}
	cout<<ans<<'\n';
	return 0;
}
