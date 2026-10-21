//
// 题目：P1845 影像之结构化特征 暴力（Bellman-Ford 求最短路层数）
// https://www.luogu.com.cn/problem/P1845
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{45};
constexpr int dx[4]{-1,1,0,0};
constexpr int dy[4]{0,0,-1,1};
constexpr int inf{1145141919};

int n,g[mxn][mxn],vis[mxn][mxn];
int xs[mxn*mxn],ys[mxn*mxn],dist[mxn*mxn];
int stepv[mxn*mxn];

int relax(int cnt,int sid)
{
	for(int i=0;i<cnt;++i)
		dist[i]=inf;
	dist[sid]=0;
	for(int it=0;it<cnt-1;++it)
	{
		bool chg=false;
		for(int i=0;i<cnt;++i)
		{
			if(dist[i]>=inf)
				continue;
			for(int k=0;k<4;++k)
			{
				int nx=xs[i]+dx[k],ny=ys[i]+dy[k];
				if(nx<0||ny<0||nx>=n||ny>=n||vis[nx][ny]==0)
					continue;
				int id=vis[nx][ny]-1;
				if(dist[i]+1<dist[id])
				{
					dist[id]=dist[i]+1;
					chg=true;
				}
			}
		}
		if(!chg)
			break;
	}
	int best=0;
	for(int i=0;i<cnt;++i)
		if(dist[i]>best)
			best=dist[i];
	return best+1;
}

void solve()
{
	memset(g,0,sizeof(g));
	memset(vis,0,sizeof(vis));
	string line;
	getline(cin,line);
	for(int i=0;i<n;++i)
	{
		getline(cin,line);
		int j=0;
		for(char c:line)
		{
			if((c=='0'||c=='1')&&j<n)
				g[i][j++]=c-'0';
		}
	}
	int tot=0;
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<n;++j)
		{
			if(!g[i][j]||vis[i][j])
				continue;
			int cnt=0,sid=0;
			int h=0,t=0;
			int qx[mxn*mxn],qy[mxn*mxn];
			qx[t]=i;
			qy[t]=j;
			++t;
			vis[i][j]=1;
			xs[cnt]=i;
			ys[cnt]=j;
			++cnt;
			while(h<t)
			{
				int x=qx[h],y=qy[h];
				++h;
				for(int k=0;k<4;++k)
				{
					int nx=x+dx[k],ny=y+dy[k];
					if(nx<0||ny<0||nx>=n||ny>=n)
						continue;
					if(!g[nx][ny]||vis[nx][ny])
						continue;
					vis[nx][ny]=cnt+1;
					xs[cnt]=nx;
					ys[cnt]=ny;
					++cnt;
					qx[t]=nx;
					qy[t]=ny;
					++t;
				}
			}
			stepv[tot++]=relax(cnt,sid);
		}
	}
	sort(stepv,stepv+tot);
	cout<<tot<<'\n';
	for(int i=0;i<tot;++i)
		cout<<stepv[i]<<'\n';
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	while(cin>>n)
		solve();
	return 0;
}
