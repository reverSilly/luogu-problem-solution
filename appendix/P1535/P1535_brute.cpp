#include<bits/stdc++.h>
using namespace std;
using i64=long long;

// 独立基准：直接枚举所有长度为 T 的行走序列(每步必动),数落在终点的条数。
int n,m,t,r2,c2;
char g[105][105];
i64 ans;
int dx[4]{-1,1,0,0},dy[4]{0,0,-1,1};

void dfs(int i,int j,int s)
{
	if(s==t)
	{
		if(i==r2&&j==c2)
			++ans;
		return;
	}
	for(int d=0;d<4;++d)
	{
		int ni=i+dx[d],nj=j+dy[d];
		if(ni<0||ni>=n||nj<0||nj>=m||g[ni][nj]=='*')
			continue;
		dfs(ni,nj,s+1);
	}
}

int main()
{
	cin>>n>>m>>t;
	for(int i=0;i<n;++i)
		cin>>g[i];
	int r1,c1;
	cin>>r1>>c1>>r2>>c2;
	--r1;--c1;--r2;--c2;
	dfs(r1,c1,0);
	cout<<ans<<'\n';
	return 0;
}
