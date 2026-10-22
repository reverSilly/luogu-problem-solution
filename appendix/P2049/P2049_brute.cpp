// 题目：P2049 魔术棋子
// 暴力：DFS 枚举所有只向右/向下的路径，累乘并记录 mod K 的余数
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{105};

int m,n,k;
i64 a[mxn][mxn];
bool vis[105];
vector<int>ans;

void dfs(int i,int j,i64 val)
{
	if(i==m-1&&j==n-1)
	{
		vis[val]=true;
		return;
	}
	if(i+1<m) dfs(i+1,j,val*a[i+1][j]%k);
	if(j+1<n) dfs(i,j+1,val*a[i][j+1]%k);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n>>k;
	for(int i=0;i<m;++i)
		for(int j=0;j<n;++j)
		{
			cin>>a[i][j];
			a[i][j]=(a[i][j]%k+k)%k;
		}
	dfs(0,0,a[0][0]%k);
	int cnt=0;
	for(int r=0;r<k;++r) if(vis[r]) ++cnt;
	cout<<cnt<<'\n';
	bool first=true;
	for(int r=0;r<k;++r)
		if(vis[r])
		{
			if(!first) cout<<' ';
			first=false;
			cout<<r;
		}
	cout<<'\n';
	return 0;
}
