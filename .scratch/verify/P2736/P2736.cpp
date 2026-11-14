#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25},inf{1145141919};

int n,t,m,a[mxn],f[mxn][mxn][mxn];
// f[i][j][k]：前 i 首歌，恰好开了 j 张 CD，第 j 张已用 k 分钟，最多选歌数

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>t>>m;
	for(int i=1;i<=n;++i)
		cin>>a[i];
	for(int i=0;i<=n;++i)
		for(int j=0;j<=m;++j)
			for(int k=0;k<=t;++k)
				f[i][j][k]=-inf;
	f[0][0][0]=0;
	for(int i=1;i<=n;++i)
		for(int j=0;j<=m;++j)
			for(int k=0;k<=t;++k)
			{
				if(f[i-1][j][k]==-inf)
					continue;
				int v=f[i-1][j][k];
				f[i][j][k]=max(f[i][j][k],v);			// 不选第 i 首
				if(j>=1&&k+a[i]<=t)						// 放进第 j 张
					f[i][j][k+a[i]]=max(f[i][j][k+a[i]],v+1);
				if(j<m&&a[i]<=t)						// 新开第 j+1 张
					f[i][j+1][a[i]]=max(f[i][j+1][a[i]],v+1);
			}
	int ans=0;
	for(int j=0;j<=m;++j)
		for(int k=0;k<=t;++k)
			ans=max(ans,f[n][j][k]);
	cout<<ans<<'\n';
	return 0;
}