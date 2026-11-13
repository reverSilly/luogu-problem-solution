#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{155};

int n,m;
char g[mxn][mxn];
int dis[mxn][mxn],Q[mxn*mxn];
const int dx[4]={-1,1,0,0};
const int dy[4]={0,0,-1,1};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
			cin>>g[i][j];
	memset(dis,-1,sizeof(dis));
	int hd=0,tl=0;
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
			if(g[i][j]=='1')
			{
				dis[i][j]=0;
				Q[tl++]=i*m+j;
			}
	while(hd<tl)
	{
		int u=Q[hd++],x=u/m,y=u%m;
		for(int k=0;k<4;++k)
		{
			int tx=x+dx[k],ty=y+dy[k];
			if(tx<0||tx>=n||ty<0||ty>=m)
				continue;
			if(dis[tx][ty]!=-1)
				continue;
			dis[tx][ty]=dis[x][y]+1;
			Q[tl++]=tx*m+ty;
		}
	}
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<m;++j)
			cout<<dis[i][j]<<' ';
		cout<<'\n';
	}
	return 0;
}
