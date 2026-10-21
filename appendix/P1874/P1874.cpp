//
// 题目：P1874 快速求和
// https://www.luogu.com.cn/problem/P1874
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxl{45};
constexpr int mxv{100005};
constexpr int inf{1145141919};

int dp[mxl][mxv];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	string s;
	int tar;
	cin>>s>>tar;
	int len=(int)s.size();
	for(int i=0;i<=len;++i)
		fill_n(dp[i],tar+1,inf);
	dp[0][0]=0;
	for(int i=0;i<len;++i)
	{
		for(int sum=0;sum<=tar;++sum)
		{
			if(dp[i][sum]==inf)
				continue;
			i64 val=0;
			for(int j=i;j<len;++j)
			{
				val=val*10+(s[j]-'0');
				if(val>tar)
					break;
				int ns=sum+(int)val;
				if(ns>tar)
					break;
				dp[j+1][ns]=min(dp[j+1][ns],dp[i][sum]+1);
			}
		}
	}
	if(dp[len][tar]>=inf)
		cout<<-1<<'\n';
	else
		cout<<dp[len][tar]-1<<'\n';
	return 0;
}
