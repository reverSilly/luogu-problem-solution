#include<bits/stdc++.h>
using namespace std;

long long s[205][205];

int main()
{
	int n,m;
	cin>>n>>m;
	for(int i=1;i<=n;++i)
		for(int j=1;j<=m;++j)
		{
			long long x;
			cin>>x;
			s[i][j]=s[i-1][j]+s[i][j-1]-s[i-1][j-1]+x;
		}
	int ans=0;
	for(int r1=1;r1<=n;++r1)
		for(int r2=r1;r2<=n;++r2)
			for(int c1=1;c1<=m;++c1)
				for(int c2=c1;c2<=m;++c2)
				{
					long long sum=s[r2][c2]-s[r1-1][c2]-s[r2][c1-1]+s[r1-1][c1-1];
					if(sum>0)
						ans=max(ans,(r2-r1+1)*(c2-c1+1));
				}
	cout<<ans<<'\n';
	return 0;
}
