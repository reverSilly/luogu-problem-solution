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
		return true;
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
				continue;
			vis[nx][ny]=true;
			if(mk[nx][ny])
				++cnt;
			qx[tail]=nx;qy[tail++]=ny;
		}
	}
	return cnt==tot;
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
	int l=0,r=1000000000;
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