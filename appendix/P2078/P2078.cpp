#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{20005};

int n,m,p,q;
//f[i]：并查集父节点
int f[mxn];
//sz[i]：以 i 为根的连通块人数
int sz[mxn];

int getrt(int x)
{
	if(f[x]==x)
		return x;
	return f[x]=getrt(f[x]);
}

//男编号 i -> i，女编号 -i -> n+i
int mp(int x)
{
	return x>0?x:n-x;
}

void uni(int u,int v)
{
	u=getrt(u);
	v=getrt(v);
	if(u==v)
		return;
	f[v]=u;
	sz[u]+=sz[v];
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>p>>q;
	for(int i=1;i<=n+m;++i)
	{
		f[i]=i;
		sz[i]=1;
	}
	for(int i=1,u,v;i<=p;++i)
	{
		cin>>u>>v;
		uni(mp(u),mp(v));
	}
	for(int i=1,u,v;i<=q;++i)
	{
		cin>>u>>v;
		uni(mp(u),mp(v));
	}
	//小明所在块（男）与小红所在块（女）互相都是朋友，配对数取较小者
	int a=sz[getrt(1)];
	int b=sz[getrt(n+1)];
	cout<<min(a,b)<<'\n';
	return 0;
}
