#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{455};

int val[mxn];
bitset<mxn>adj[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n,m;
	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>val[i];
	for(int i=0;i<m;++i)
	{
		int a,b;
		cin>>a>>b;
		--a;--b;
		adj[a][b]=adj[b][a]=1;
	}

	int ans=0;
	for(int i=0;i<n;++i)
		ans=max(ans,val[i]);

	//铁索两两不相交 => 平面图 => 团的大小至多 4
	for(int u=0;u<n;++u)
		for(int v=u+1;v<n;++v)
		{
			if(!adj[u][v])
				continue;
			ans=max(ans,val[u]+val[v]);
			bitset<mxn>common=adj[u]&adj[v];
			for(int w=v+1;w<n;++w)
			{
				if(!common[w])
					continue;
				ans=max(ans,val[u]+val[v]+val[w]);
				bitset<mxn>three=common&adj[w];
				for(int z=w+1;z<n;++z)
					if(three[z])
						ans=max(ans,val[u]+val[v]+val[w]+val[z]);
			}
		}
	cout<<ans<<'\n';
	return 0;
}
