#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{205};

int n,m;
i64 s[mxn][mxn];                 // 二维前缀和(1-based)

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
		{
			i64 x;
			cin>>x;
			s[i+1][j+1]=s[i][j+1]+s[i+1][j]-s[i][j]+x;
		}
	int ans=0;
	for(int r1=0;r1<n;++r1)
		for(int r2=r1+1;r2<=n;++r2)
			for(int c1=0;c1<m;++c1)
				for(int c2=c1+1;c2<=m;++c2)
				{
					i64 sum=s[r2][c2]-s[r1][c2]-s[r2][c1]+s[r1][c1];
					if(sum>0)
						ans=max(ans,(r2-r1)*(c2-c1));
				}
	cout<<ans<<'\n';
	return 0;
}
