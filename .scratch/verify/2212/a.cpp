#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2005};
constexpr i64 inf{1145141919810};

int n,c;
i64 x[mxn],y[mxn];
i64 dis[mxn];       // dis[v]=当前生成树到v的最短合法边权
bool vis[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>c;
	for(int i=0;i<n;++i)
		cin>>x[i]>>y[i];

	fill_n(dis,mxn,inf);
	dis[0]=0;
	i64 ans=0;
	for(int it=0;it<n;++it)
	{
		int u=-1;
		for(int i=0;i<n;++i)
			if(!vis[i]&&(u==-1||dis[i]<dis[u]))
				u=i;
		if(dis[u]>=inf)
		{
			cout<<-1<<'\n';
			return 0;
		}
		vis[u]=1;
		ans+=dis[u];
		for(int v=0;v<n;++v)
			if(!vis[v])
			{
				i64 w=(x[u]-x[v])*(x[u]-x[v])+(y[u]-y[v])*(y[u]-y[v]);
				if(w>=c&&w<dis[v])
					dis[v]=w;
			}
	}
	cout<<ans<<'\n';
	return 0;
}
