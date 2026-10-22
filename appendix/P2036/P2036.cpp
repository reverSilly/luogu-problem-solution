// 题目：P2036 [COCI 2008/2009 #2] PERKET
// 枚举配料子集：n<=10，直接暴力枚举 2^n 种选法
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{15};
constexpr i64 inf{1LL<<62};

int n;
i64 s[mxn],b[mxn];
i64 ans=inf;

void dfs(int i,i64 prod,i64 sum,bool used)
{
	if(i==n)
	{
		if(used) ans=min(ans,llabs(prod-sum));
		return;
	}
	dfs(i+1,prod,sum,used);			// 不选第 i 种
	dfs(i+1,prod*s[i],sum+b[i],true);	// 选第 i 种
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i) cin>>s[i]>>b[i];
	dfs(0,1,0,false);
	cout<<ans<<'\n';
	return 0;
}
