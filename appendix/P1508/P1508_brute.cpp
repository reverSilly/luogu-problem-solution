//
// 题目：P1508 Likecloud-吃、吃、吃 (暴力基准)
// https://www.luogu.com.cn/problem/P1508
// 仅用于小规模对拍: 枚举全部 3^m 条路径取最大能量
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25};
constexpr int inf{1145141919};

int m,n;
int a[mxn][mxn];
int ans;

void dfs(int i,int j,int sum)
{
	if(i==0)
	{
		ans=max(ans,sum);
		return;
	}
	for(int k=-1;k<=1;++k)
	{
		int c=j+k;
		if(c<0||c>=n)
			continue;
		dfs(i-1,c,sum+a[i-1][c]);
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n;
	for(int i=0;i<m;++i)
		for(int j=0;j<n;++j)
			cin>>a[i][j];

	ans=-inf;
	dfs(m,n>>1,0);
	cout<<ans<<'\n';
	return 0;
}
