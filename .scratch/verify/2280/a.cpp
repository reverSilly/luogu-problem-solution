#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{5005};

int n,m,a[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0,x,y,v;i<n;++i)
	{
		cin>>x>>y>>v;
		a[x+1][y+1]+=v;
	}
	for(int i=1;i<=5001;++i)
		for(int j=1;j<=5001;++j)
			a[i][j]+=a[i-1][j]+a[i][j-1]-a[i-1][j-1];
	if(m>5001)
		m=5001;
	int ans=0;
	for(int i=m;i<=5001;++i)
		for(int j=m;j<=5001;++j)
			ans=max(ans,a[i][j]-a[i-m][j]-a[i][j-m]+a[i-m][j-m]);
	cout<<ans<<'\n';
	return 0;
}
