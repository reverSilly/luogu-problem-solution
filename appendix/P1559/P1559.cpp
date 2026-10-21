//
// 题目：P1559 运动员最佳匹配问题
// https://www.luogu.com.cn/problem/P1559
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25};            // n<=20 (+5 松弛)
constexpr int inf{0x3f3f3f3f};

int n;
int p[mxn][mxn],q[mxn][mxn],w[mxn][mxn];
int lx[mxn],ly[mxn],match_y[mxn],slack[mxn],pre[mxn];
bool vis_y[mxn];

void bfs_augment(int start_x)
{
	memset(vis_y,0,sizeof(vis_y));
	fill(slack,slack+n+1,inf);
	int cur_y=0;
	match_y[0]=start_x;
	do
	{
		vis_y[cur_y]=true;
		int cur_x=match_y[cur_y],delta=inf,nxt_y=0;
		for(int y=1;y<=n;++y)
		{
			if(vis_y[y])
				continue;
			int d=lx[cur_x]+ly[y]-w[cur_x][y];
			if(d<slack[y])
			{
				slack[y]=d;
				pre[y]=cur_y;
			}
			if(slack[y]<delta)
			{
				delta=slack[y];
				nxt_y=y;
			}
		}
		for(int y=0;y<=n;++y)
		{
			if(vis_y[y])
			{
				lx[match_y[y]]-=delta;
				ly[y]+=delta;
			}
			else
				slack[y]-=delta;
		}
		cur_y=nxt_y;
	}while(match_y[cur_y]!=0);

	while(cur_y)
	{
		match_y[cur_y]=match_y[pre[cur_y]];
		cur_y=pre[cur_y];
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n))
		return 0;
	for(int i=1;i<=n;++i)
		for(int j=1;j<=n;++j)
			cin>>p[i][j];
	for(int i=1;i<=n;++i)
		for(int j=1;j<=n;++j)
			cin>>q[i][j];

	for(int i=1;i<=n;++i)
	{
		lx[i]=-inf;
		ly[i]=0;
		for(int j=1;j<=n;++j)
		{
			w[i][j]=p[i][j]*q[j][i];
			lx[i]=max(lx[i],w[i][j]);
		}
	}

	for(int i=1;i<=n;++i)
		bfs_augment(i);

	int ans=0;
	for(int y=1;y<=n;++y)
		ans+=w[match_y[y]][y];
	cout<<ans<<'\n';
	return 0;
}
