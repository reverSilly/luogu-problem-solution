// P2658 暴力：Floyd 求所有点对的 minimax 路径，答案=路标点对间 minimax 的最大值
#include<bits/stdc++.h>
using namespace std;
int m,n,h[25][25],mk[25][25],d[625][625];
int main()
{
	cin>>m>>n;
	for(int i=0;i<m;++i)for(int j=0;j<n;++j)cin>>h[i][j];
	for(int i=0;i<m;++i)for(int j=0;j<n;++j)cin>>mk[i][j];
	int V=m*n;
	auto id=[&](int i,int j){return i*n+j;};
	for(int i=0;i<V;++i)for(int j=0;j<V;++j)d[i][j]=(i==j?0:1000000001);
	int dx[4]={1,-1,0,0},dy[4]={0,0,1,-1};
	for(int i=0;i<m;++i)for(int j=0;j<n;++j)for(int k=0;k<4;++k)
	{
		int ni=i+dx[k],nj=j+dy[k];
		if(ni<0||ni>=m||nj<0||nj>=n)continue;
		int w=abs(h[ni][nj]-h[i][j]);
		d[id(i,j)][id(ni,nj)]=min(d[id(i,j)][id(ni,nj)],w);
	}
	for(int k=0;k<V;++k)for(int i=0;i<V;++i)for(int j=0;j<V;++j)
		d[i][j]=min(d[i][j],max(d[i][k],d[k][j]));
	vector<int>ls;
	for(int i=0;i<m;++i)for(int j=0;j<n;++j)if(mk[i][j])ls.push_back(id(i,j));
	int ans=0;
	for(int x:ls)for(int y:ls)ans=max(ans,d[x][y]);
	cout<<ans<<'\n';
	return 0;
}