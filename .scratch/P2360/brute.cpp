// DFS 枚举所有简单路径取最短, 完全独立于 BFS
#include<bits/stdc++.h>
using namespace std;
int L,R,C;
char mp[4][4][4];
int sx,sy,sz,ex,ey,ez;
int best;
bool vis[4][4][4];
int dx[6]={1,-1,0,0,0,0},dy[6]={0,0,1,-1,0,0},dz[6]={0,0,0,0,1,-1};
void dfs(int x,int y,int z,int d)
{
	if(d>=best)return;
	if(x==ex&&y==ey&&z==ez){best=d;return;}
	for(int i=0;i<6;++i)
	{
		int nx=x+dx[i],ny=y+dy[i],nz=z+dz[i];
		if(nx<0||nx>=L||ny<0||ny>=R||nz<0||nz>=C)continue;
		if(mp[nx][ny][nz]=='#'||vis[nx][ny][nz])continue;
		vis[nx][ny][nz]=1;
		dfs(nx,ny,nz,d+1);
		vis[nx][ny][nz]=0;
	}
}
int main()
{
	cin>>L>>R>>C;
	for(int i=0;i<L;++i)
		for(int j=0;j<R;++j)
			for(int k=0;k<C;++k)
			{
				cin>>mp[i][j][k];
				if(mp[i][j][k]=='S'){sx=i;sy=j;sz=k;}
				if(mp[i][j][k]=='E'){ex=i;ey=j;ez=k;}
			}
	best=1e9;
	vis[sx][sy][sz]=1;
	dfs(sx,sy,sz,0);
	if(best>1e8)puts("Trapped!");
	else printf("Escaped in %d minute(s).\n",best);
	return 0;
}
