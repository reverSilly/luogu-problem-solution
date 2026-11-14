#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int n,f[3],g[3];
bool vis[mxn][mxn][mxn];

int wrap(int v){return (v%n+n)%n;}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<3;++i){cin>>f[i];--f[i];}
	for(int i=0;i<3;++i){cin>>g[i];--g[i];}
	int ans=0;
	for(int t=0;t<2;++t)
	{
		int*c=t?g:f;
		for(int dx=-2;dx<=2;++dx)
			for(int dy=-2;dy<=2;++dy)
				for(int dz=-2;dz<=2;++dz)
				{
					int a=wrap(c[0]+dx),b=wrap(c[1]+dy),d=wrap(c[2]+dz);
					if(!vis[a][b][d])
					{
						vis[a][b][d]=true;
						++ans;
					}
				}
	}
	cout<<ans<<'\n';
	return 0;
}