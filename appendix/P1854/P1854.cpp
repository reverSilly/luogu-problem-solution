//
// 题目：P1854 [IOI 1999] 花店橱窗布置
// https://www.luogu.com.cn/problem/P1854
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};
constexpr int inf{1145141919};

int f,v;
int a[mxn][mxn];
int dp[mxn][mxn];
int pre[mxn][mxn];
int anspos[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>f>>v;
	for(int i=1;i<=f;++i)
		for(int j=1;j<=v;++j)
			cin>>a[i][j];

	for(int i=0;i<=f;++i)
		for(int j=0;j<=v;++j)
			dp[i][j]=-inf;
	dp[0][0]=0;

	for(int i=1;i<=f;++i)
	{
		for(int j=i;j<=v-(f-i);++j)
		{
			for(int k=i-1;k<j;++k)
			{
				if(dp[i-1][k]==-inf)
					continue;
				int cur=dp[i-1][k]+a[i][j];
				if(cur>dp[i][j])
				{
					dp[i][j]=cur;
					pre[i][j]=k;
				}
			}
		}
	}

	int best=-inf,bj=0;
	for(int j=f;j<=v;++j)
		if(dp[f][j]>best)
		{
			best=dp[f][j];
			bj=j;
		}

	for(int i=f;i>=1;--i)
	{
		anspos[i]=bj;
		bj=pre[i][bj];
	}

	cout<<best<<'\n';
	for(int i=1;i<=f;++i)
	{
		if(i>1)
			cout<<' ';
		cout<<anspos[i];
	}
	cout<<'\n';
	return 0;
}
