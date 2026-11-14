// 题目：P2658 汽车拉力比赛
// 思路：固定 D 后"差<=D 相邻连边"，可达是等价关系 ⇒ 所有路标两两可达 ⟺ 同一连通块。
//       ok(D) 单调（放宽只增边）⇒ 二分最小可行 D，check 用一次 BFS 数路标。
//       等价视角：D* = 最小生成树上路标间路径的最大边权（Kruskal 瓶颈）。
//       注意输入是 M 行 N 列（先 m 后 n）。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{505};

int n,m,h[mxn][mxn],tot,sx,sy;
bool mk[mxn][mxn],vis[mxn][mxn];
int qx[mxn*mxn],qy[mxn*mxn];
int dx[4]={1,-1,0,0},dy[4]={0,0,1,-1};

bool check(int D)
{
	if(tot<=1)
		return true;	// 零个或一个路标，D=0 即可
	memset(vis,0,sizeof vis);
	int head=0,tail=0,cnt=0;
	qx[tail]=sx;qy[tail++]=sy;
	vis[sx][sy]=true;
	if(mk[sx][sy])
		++cnt;
	while(head<tail)
	{
		int x=qx[head],y=qy[head++];
		for(int d=0;d<4;++d)
		{
			int nx=x+dx[d],ny=y+dy[d];
			if(nx<0||nx>=m||ny<0||ny>=n||vis[nx][ny])
				continue;
			if(abs(h[nx][ny]-h[x][y])>D)
				continue;	// 海拔差超过阈值，这条边不可用
			vis[nx][ny]=true;
			if(mk[nx][ny])
				++cnt;
			qx[tail]=nx;qy[tail++]=ny;
		}
	}
	return cnt==tot;	// 所有路标都在源点连通块里
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>m>>n;	// 题面：第一行 M N，M 行 N 列
	for(int i=0;i<m;++i)
		for(int j=0;j<n;++j)
			cin>>h[i][j];
	for(int i=0;i<m;++i)
		for(int j=0;j<n;++j)
		{
			cin>>mk[i][j];
			if(mk[i][j])
			{
				++tot;
				if(tot==1)
					sx=i,sy=j;
			}
		}
	int l=0,r=1000000000;	// 海拔差上界 1e9，check(1e9) 必真
	while(l<r)
	{
		int mid=(l+r)>>1;
		if(check(mid))
			r=mid;
		else
			l=mid+1;
	}
	cout<<l<<'\n';
	return 0;
}
