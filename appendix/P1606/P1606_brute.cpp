//
// P1606 暴力: 枚举所有水格子集, 统计每层大小下"加上该子集后 S 能到 T"的方案数,
// 取最小的有解层
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{35};

int M,N;
int g[mxn][mxn];
int wx[420],wy[420],W;
int sx,sy,tx,ty;
constexpr int dx[8]{1,1,2,2,-1,-1,-2,-2};
constexpr int dy[8]{2,-2,1,-1,2,-2,1,-1};

bool vis[mxn][mxn];
int qx[420],qy[420];

bool reach(int mask)
{
	for(int i=1;i<=M;++i)
		for(int j=1;j<=N;++j)
			vis[i][j]=false;
	int ql=0,qr=0;
	qx[qr]=sx,qy[qr++]=sy;
	vis[sx][sy]=true;
	while(ql<qr)
	{
		int x=qx[ql],y=qy[ql];
		++ql;
		if(x==tx&&y==ty)
			return true;
		for(int k=0;k<8;++k)
		{
			int nx=x+dx[k],ny=y+dy[k];
			if(nx<1||nx>M||ny<1||ny>N||g[nx][ny]==2||vis[nx][ny])
				continue;
			if(g[nx][ny]==0)                   // 水格必须已被放上荷叶
			{
				int id=-1;
				for(int i=0;i<W;++i)
					if(wx[i]==nx&&wy[i]==ny)
						id=i;
				if(id<0||!(mask>>id&1))
					continue;
			}
			vis[nx][ny]=true;
			qx[qr]=nx,qy[qr++]=ny;
		}
	}
	return false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>M>>N;
	for(int i=1;i<=M;++i)
		for(int j=1;j<=N;++j)
		{
			cin>>g[i][j];
			if(g[i][j]==3)
				sx=i,sy=j;
			if(g[i][j]==4)
				tx=i,ty=j;
			if(g[i][j]==0)
				wx[W]=i,wy[W]=j,++W;
		}
	if(W>22)
	{
		cout<<"TOOBIG\n";
		return 0;
	}

	int lim=1<<W;
	vector<i64>cnt(W+1,0);
	for(int mask=0;mask<lim;++mask)
		if(reach(mask))
			++cnt[__builtin_popcount((unsigned)mask)];
	for(int k=0;k<=W;++k)
		if(cnt[k])
		{
			cout<<k<<'\n'<<cnt[k]<<'\n';
			return 0;
		}
	cout<<-1<<'\n';
	return 0;
}
