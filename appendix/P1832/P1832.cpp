//
// 题目：P1832 A+B Problem（再升级）
// https://www.luogu.com.cn/problem/P1832
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005};

int pr[mxn],pc;
bool comp[mxn];
i64 dp[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int n;
	cin>>n;
	for(int i=2;i<=n;++i)
	{
		if(!comp[i])
			pr[pc++]=i;
		for(int j=0;j<pc&&i*1LL*pr[j]<=n;++j)
		{
			comp[i*pr[j]]=true;
			if(i%pr[j]==0)
				break;
		}
	}
	dp[0]=1;
	for(int i=0;i<pc;++i)
		for(int j=pr[i];j<=n;++j)
			dp[j]+=dp[j-pr[i]];
	cout<<dp[n]<<'\n';
	return 0;
}
