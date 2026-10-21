//
// 题目：P1606 [USACO07FEB] Lilypad Pond G
// https://www.luogu.com.cn/problem/P1606
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{35};                    // M,N<=30 (+5 松弛)
constexpr int inf{1145141919};
constexpr int dx[8]{1,1,2,2,-1,-1,-2,-2},dy[8]{2,-2,1,-1,2,-2,1,-1};

int M,N,g[mxn][mxn],dis[mxn][mxn],sx,sy,tx,ty;
int fa[mxn*mxn];                          // 已有荷叶的并查集
int adjc[mxn][mxn][8],adjcn[mxn][mxn];    // 水格邻域中荷叶连通块(去重)
i64 dp[mxn][mxn];                         // 恰好用 dis 片荷叶到达该水格的方案数

int getrt(int x){return fa[x]==x?x:fa[x]=getrt(fa[x]);}

bool lily(int v){return v==1||v==3||v==4;}

bool kn(int x1,int y1,int x2,int y2)
{
	int a=abs(x1-x2),b=abs(y1-y2);
	return (a==1&&b==2)||(a==2&&b==1);
}

bool hasc(int x,int y,int c)              // 水格能否只踩已有荷叶走到连通块 c
{
	for(int i=0;i<adjcn[x][y];++i)
		if(adjc[x][y][i]==c)
			return true;
	return false;
}

bool compat(int x1,int y1,int x2,int y2)  // 两水格之间能否不加水直接连通
{
	if(kn(x1,y1,x2,y2))
		return true;
	for(int i=0;i<adjcn[x1][y1];++i)
		for(int j=0;j<adjcn[x2][y2];++j)
			if(adjc[x1][y1][i]==adjc[x2][y2][j])
				return true;
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
			if(g[i][j]==3)sx=i,sy=j;
			if(g[i][j]==4)tx=i,ty=j;
			dis[i][j]=inf;
		}

	for(int i=1;i<=M;++i)                 // 并查集只含已有荷叶(1/3/4)
		for(int j=1;j<=N;++j)
			if(lily(g[i][j]))
				fa[i*N+j]=i*N+j;
	for(int i=1;i<=M;++i)                 // 按马步合并连通块
		for(int j=1;j<=N;++j)
		{
			if(!lily(g[i][j]))
				continue;
			for(int k=0;k<8;++k)
			{
				int x=i+dx[k],y=j+dy[k];
				if(x<1||x>M||y<1||y>N||!lily(g[x][y]))
					continue;
				int u=getrt(i*N+j),v=getrt(x*N+y);
				if(u!=v)
					fa[u]=v;
			}
		}

	deque<pair<int,int>>Q;                // 0-1 BFS: 落水格代价 1, 落荷叶代价 0
	dis[sx][sy]=0;
	Q.push_front({sx,sy});
	while(!Q.empty())
	{
		auto[x,y]=Q.front();
		Q.pop_front();
		for(int k=0;k<8;++k)
		{
			int nx=x+dx[k],ny=y+dy[k];
			if(nx<1||nx>M||ny<1||ny>N||g[nx][ny]==2)
				continue;
			int w=g[nx][ny]==0?1:0;
			if(dis[x][y]+w<dis[nx][ny])
			{
				dis[nx][ny]=dis[x][y]+w;
				if(w==0)
					Q.push_front({nx,ny});
				else
					Q.push_back({nx,ny});
			}
		}
	}

	int d=dis[tx][ty];
	if(d==inf)
	{
		cout<<-1<<'\n';
		return 0;
	}
	if(d==0)                              // 全靠已有荷叶可达, 唯一方案是不加
	{
		cout<<0<<'\n'<<1<<'\n';
		return 0;
	}

	for(int i=1;i<=M;++i)                 // 每个水格"免费"能触及的荷叶连通块
		for(int j=1;j<=N;++j)
		{
			if(g[i][j]!=0)
				continue;
			for(int k=0;k<8;++k)
			{
				int x=i+dx[k],y=j+dy[k];
				if(x<1||x>M||y<1||y>N||!lily(g[x][y]))
					continue;
				int c=getrt(x*N+y),dup=0;
				for(int q=0;q<adjcn[i][j];++q)
					if(adjc[i][j][q]==c)
						dup=1;
				if(!dup)
					adjc[i][j][adjcn[i][j]++]=c;
			}
		}

	int cs=getrt(sx*N+sy),ct=getrt(tx*N+ty);
	for(int k=1;k<=d;++k)                 // 按 dis 分层, 第 k 层恰好用 k 片新荷叶
		for(int i=1;i<=M;++i)
			for(int j=1;j<=N;++j)
			{
				if(g[i][j]!=0||dis[i][j]!=k)
					continue;
				i64 s=0;
				if(k==1)
					s=hasc(i,j,cs)?1:0;
				else
					for(int x=1;x<=M;++x)
						for(int y=1;y<=N;++y)
							if(g[x][y]==0&&dis[x][y]==k-1&&dp[x][y]&&compat(x,y,i,j))
								s+=dp[x][y];
				dp[i][j]=s;
			}

	i64 ans=0;
	for(int i=1;i<=M;++i)
		for(int j=1;j<=N;++j)
			if(g[i][j]==0&&dis[i][j]==d&&dp[i][j]&&hasc(i,j,ct))
				ans+=dp[i][j];
	cout<<d<<'\n'<<ans<<'\n';
	return 0;
}
