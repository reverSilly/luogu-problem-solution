// 题目：P2427 Wave
// 思路：波速不变 ⇔ 正方形全为源字母；中心正方形边长 2k+1
//       可行性关于半径单调 ⇒ 二分；二维前缀和 O(1) 判"全为 c"
//       按源字母离线分组、复用一张前缀表，避免 26 张表爆内存
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005},mxq{10005};

int n,m,q;
char g[mxn][mxn];
int s[mxn][mxn];
int ans[mxq];
struct query{int x,y,id;};
vector<query>qs[26];

// is the whole square [x-k,x+k]x[y-k,y+k] made of letter c ?
bool ok(int x,int y,int k)
{
	int r1=x-k,r2=x+k,c1=y-k,c2=y+k;
	int cnt=s[r2+1][c2+1]-s[r1][c2+1]-s[r2+1][c1]+s[r1][c1];
	int side=2*k+1;
	return cnt==side*side;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>q;
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
			cin>>g[i][j];
	for(int i=0,x,y;i<q;++i)
	{
		cin>>x>>y;
		qs[g[x][y]-'a'].push_back({x,y,i});
	}
	// answer queries grouped by source letter; one prefix table reused per letter
	for(int c=0;c<26;++c)
	{
		if(qs[c].empty())continue;
		memset(s,0,sizeof(s));
		for(int i=1;i<=n;++i)
			for(int j=1;j<=m;++j)
				s[i][j]=s[i-1][j]+s[i][j-1]-s[i-1][j-1]+(g[i-1][j-1]-'a'==c);
		for(auto[x,y,id]:qs[c])
		{
			int lim=min({x,y,n-1-x,m-1-y});
			int lo=0,hi=lim;
			while(lo<hi)
			{
				int mid=(lo+hi+1)>>1;
				if(ok(x,y,mid))lo=mid;
				else hi=mid-1;
			}
			ans[id]=2*lo+1;
		}
	}
	for(int i=0;i<q;++i)cout<<ans[i]<<'\n';
	return 0;
}
