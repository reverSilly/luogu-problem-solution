//
// 题目：P1514 [NOIP 2010 提高组] 引水入城
// https://www.luogu.com.cn/problem/P1514
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{505};           // N,M<=500 (+5 松弛)
constexpr int inf{1145141919};    // 114514 家族哨兵
constexpr int dr[4]{-1,1,0,0};
constexpr int dc[4]{0,0,-1,1};

int n,m;
int h[mxn][mxn];
bool vis[mxn][mxn];
int lft[mxn][mxn],rgt[mxn][mxn];  // (r,c) 可达的底行区间 [lft, rgt] (0-based 列号)

void dfs(int r,int c)
{
	vis[r][c]=true;
	if(r==n-1)
		lft[r][c]=c,rgt[r][c]=c;  // 底行自身先作为基例;注意底行之间仍可继续向更低处流动!
	for(int d=0;d<4;++d)
	{
		int nr=r+dr[d],nc=c+dc[d];
		if(nr<0||nr>=n||nc<0||nc>=m||h[nr][nc]>=h[r][c])
			continue;
		if(!vis[nr][nc])
			dfs(nr,nc);
		lft[r][c]=min(lft[r][c],lft[nr][nc]);
		rgt[r][c]=max(rgt[r][c],rgt[nr][nc]);
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
			cin>>h[i][j];

	fill_n(&lft[0][0],mxn*mxn,inf);
	fill_n(&rgt[0][0],mxn*mxn,-inf);

	for(int j=0;j<m;++j)
		if(!vis[0][j])
			dfs(0,j);

	int miss=0;
	for(int j=0;j<m;++j)
		if(!vis[n-1][j])
			++miss;
	if(miss>0)
	{
		cout<<0<<'\n'<<miss<<'\n';
		return 0;
	}

	// 全体底行可达 => 每个顶行点覆盖底行一个连续闭区间 [lft[0][j], rgt[0][j]], 贪心区间覆盖 [0, m-1]
	int cur=0,ans=0;
	while(cur<m)
	{
		int mxr=-1;
		for(int j=0;j<m;++j)
			if(lft[0][j]<=cur)
				mxr=max(mxr,rgt[0][j]);
		++ans;
		cur=mxr+1;
	}
	cout<<1<<'\n'<<ans<<'\n';
	return 0;
}
