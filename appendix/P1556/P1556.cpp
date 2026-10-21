//
// 题目：P1556 [USACO12MAR] 幸福的路 Connect the Cows B
// https://www.luogu.com.cn/problem/P1556
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{15};

int n;
int x[mxn],y[mxn];
int dir_to[mxn][mxn];             // dir_to[u][v]: 从 u 到 v 的方向 (0..3), 不可直达则为 -1
int dp[1<<10][11][4];

int get_dir(int u,int v)
{
	if(x[u]==x[v]&&y[u]==y[v])
		return -1;
	if(x[u]==x[v])
		return y[v]>y[u]?0:1;
	if(y[u]==y[v])
		return x[v]>x[u]?2:3;
	return -1;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n))
		return 0;
	x[0]=0;y[0]=0;
	for(int i=1;i<=n;++i)
		cin>>x[i]>>y[i];

	for(int i=0;i<=n;++i)
		for(int j=0;j<=n;++j)
			dir_to[i][j]=get_dir(i,j);

	for(int i=1;i<=n;++i)
		if(dir_to[0][i]!=-1)
			dp[1<<(i-1)][i][dir_to[0][i]]=1;

	int full=(1<<n)-1;
	for(int mask=1;mask<full;++mask)
		for(int u=1;u<=n;++u)
		{
			if(!((mask>>(u-1))&1))
				continue;
			for(int d=0;d<4;++d)
			{
				int ways=dp[mask][u][d];
				if(!ways)
					continue;
				for(int v=1;v<=n;++v)
				{
					if((mask>>(v-1))&1)
						continue;
					int nd=dir_to[u][v];
					if(nd!=-1&&nd!=d)
						dp[mask|(1<<(v-1))][v][nd]+=ways;
				}
			}
		}

	long long ans=0;
	for(int u=1;u<=n;++u)
		for(int d=0;d<4;++d)
		{
			int ways=dp[full][u][d];
			if(!ways)
				continue;
			int nd=dir_to[u][0];
			if(nd!=-1&&nd!=d)
				ans+=ways;
		}

	cout<<ans<<'\n';
	return 0;
}
