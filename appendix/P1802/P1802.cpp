//
// 题目：P1802 5 倍经验日
// https://www.luogu.com.cn/problem/P1802
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005};

int n,x;
int lose[mxn],win[mxn],use[mxn];
i64 dp[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>x;
	for(int i=1;i<=n;++i)
		cin>>lose[i]>>win[i]>>use[i];
	for(int i=1;i<=n;++i)
	{
		for(int j=x;j>=0;--j)
		{
			if(j>=use[i])
				dp[j]=max(dp[j]+lose[i],dp[j-use[i]]+win[i]);
			else
				dp[j]+=lose[i];
		}
	}
	cout<<dp[x]*5<<'\n';
	return 0;
}
