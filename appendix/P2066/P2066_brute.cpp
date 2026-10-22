// 题目：P2066 机器分配
// 暴力：枚举所有分配方案（总台数不超过 M），取最大盈利；
//       平局时取字典序最小的方案（编号小的公司分得设备尽可能少）
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{12};
constexpr int mxm{17};

int n,m;
i64 v[mxn][mxm];
int best[mxn],cur[mxn];
i64 ans;

bool lexLess()
{
	for(int i=1;i<=n;++i)
		if(cur[i]!=best[i]) return cur[i]<best[i];
	return false;
}

void dfs(int i,int rest,i64 sum)
{
	if(i>n)
	{
		if(sum>ans||(sum==ans&&lexLess()))
		{
			ans=sum;
			for(int j=1;j<=n;++j) best[j]=cur[j];
		}
		return;
	}
	for(int k=0;k<=rest;++k)
	{
		cur[i]=k;
		dfs(i+1,rest-k,sum+v[i][k]);
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=1;i<=n;++i)
		for(int j=1;j<=m;++j) cin>>v[i][j];
	ans=-1;
	dfs(1,m,0);
	cout<<ans<<'\n';
	for(int i=1;i<=n;++i) cout<<i<<' '<<best[i]<<'\n';
	return 0;
}
