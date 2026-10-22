// P2194 暴力：小规模用 Floyd 求可达性，再按强连通分量统计最小点权与计数
#include<bits/stdc++.h>
using namespace std;

int n,m;
int w[25];
bool can[25][25];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1;i<=n;++i)cin>>w[i];
	cin>>m;
	for(int i=0;i<m;++i)
	{
		int x,y;cin>>x>>y;
		can[x][y]=true;
	}
	// 传递闭包（含自反）
	for(int i=1;i<=n;++i)can[i][i]=true;
	for(int k=1;k<=n;++k)
		for(int i=1;i<=n;++i)
			for(int j=1;j<=n;++j)
				can[i][j]=can[i][j]||(can[i][k]&&can[k][j]);
	int comp[25]={0};
	int cc=0;
	for(int i=1;i<=n;++i)
	{
		if(comp[i])continue;
		++cc;
		comp[i]=cc;
		for(int j=1;j<=n;++j)
			if(!comp[j]&&can[i][j]&&can[j][i])comp[j]=cc;
	}
	int mn[25],cnt[25];
	for(int i=1;i<=cc;++i)mn[i]=1145141919,cnt[i]=0;
	for(int i=1;i<=n;++i)
	{
		int c=comp[i];
		if(w[i]<mn[c])
		{
			mn[c]=w[i];
			cnt[c]=1;
		}
		else if(w[i]==mn[c])++cnt[c];
	}
	long long cost=0,ways=1;
	for(int i=1;i<=cc;++i)
	{
		cost+=mn[i];
		ways=ways*cnt[i]%1000000007;
	}
	cout<<cost<<' '<<ways<<'\n';
	return 0;
}
