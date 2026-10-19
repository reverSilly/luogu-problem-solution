// 朴素暴力:DFS 枚举每条路径 + 每个格子"是否乘 3",绝对枚举所有方案
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{20};               // 仅适用于小 n
constexpr i64 nf{-1'000'000'000'000'000'000};

int n,k;
i64 a[mxn][mxn];
i64 best;

void dfs(int r,int p,int used,i64 sum)   // sum 已含 0..r-1 行的贡献
{
	i64 s1=sum+a[r][p];                  // 本格不乘 3
	if(r==n-1)
		best=max(best,s1);
	else
	{
		dfs(r+1,p,used,s1);              // 走左下
		dfs(r+1,p+1,used,s1);            // 走右下
	}
	if(used<k)                           // 本格乘 3 (消耗一次机会)
	{
		i64 s2=sum+3*a[r][p];
		if(r==n-1)
			best=max(best,s2);
		else
		{
			dfs(r+1,p,used+1,s2);
			dfs(r+1,p+1,used+1,s2);
		}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	for(int i=0;i<n;++i)
		for(int j=0;j<=i;++j)
			cin>>a[i][j];
	best=nf;
	dfs(0,0,0,0);
	cout<<best<<'\n';
	return 0;
}
