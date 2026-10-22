// 题目：P2001 硬币的面值
// 暴力：DP 枚举「连续可拼区间」cov 的所有转移，取覆盖到 m 的最小硬币数
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{200005};

int n,m;
int a[mxn];
int val[mxn],vcnt;
int dp[mxn*4];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];
		if(a[i]<=m)
			val[vcnt++]=a[i];
	}
	sort(val,val+vcnt);
	vcnt=unique(val,val+vcnt)-val;
	if(vcnt==0||val[0]!=1)
	{
		cout<<"No answer!!!\n";
		return 0;
	}
	int mx=val[vcnt-1];
	int lim=m+mx+5;
	const int inf=0x3f3f3f3f;
	fill_n(dp,lim+1,inf);
	dp[0]=0;
	for(int c=0;c<=lim;++c)
	{
		if(dp[c]==inf)
			continue;
		for(int i=0;i<vcnt;++i)
		{
			int v=val[i];
			if(v>c+1)
				break;
			if(c+v>lim)
				break;
			dp[c+v]=min(dp[c+v],dp[c]+1);
		}
	}
	int ans=inf;
	for(int c=m;c<=lim;++c)
		ans=min(ans,dp[c]);
	if(ans==inf)
		cout<<"No answer!!!\n";
	else
		cout<<ans<<'\n';
	return 0;
}
