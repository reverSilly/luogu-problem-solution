//
// 题目：P1877 [HAOI2012] 音量调节
// https://www.luogu.com.cn/problem/P1877
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{55};
constexpr int mxv{1005};

bool dp[mxn][mxv];
int c[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int n,beg,cap;
	cin>>n>>beg>>cap;
	dp[0][beg]=true;
	for(int i=1;i<=n;++i)
	{
		cin>>c[i];
		for(int v=0;v<=cap;++v)
		{
			if(!dp[i-1][v])
				continue;
			if(v+c[i]<=cap)
				dp[i][v+c[i]]=true;
			if(v>=c[i])
				dp[i][v-c[i]]=true;
		}
	}
	int ans=-1;
	for(int v=cap;v>=0;--v)
	{
		if(dp[n][v])
		{
			ans=v;
			break;
		}
	}
	cout<<ans<<'\n';
	return 0;
}
