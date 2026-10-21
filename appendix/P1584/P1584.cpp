//
// 题目：P1584 魔杖
// https://www.luogu.com.cn/problem/P1584
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};          // n<=1000 (+5 松弛)

int n;
long long low_len,high_len;
long long pref_l[mxn],pref_m[mxn];
long long best[mxn][mxn];         // best[i][j]: 左端点 <= i 且右端点 <= j 的互不包含魔杖链最大魔力和

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>low_len>>high_len))
		return 0;
	for(int i=1;i<=n;++i)
	{
		long long x;
		cin>>x;
		pref_l[i]=pref_l[i-1]+x;
	}
	for(int i=1;i<=n;++i)
	{
		long long x;
		cin>>x;
		pref_m[i]=pref_m[i-1]+x;
	}

	for(int i=1;i<=n;++i)
		for(int j=1;j<=n;++j)
		{
			best[i][j]=max(best[i-1][j],best[i][j-1]);
			if(i<=j)
			{
				long long len=pref_l[j]-pref_l[i-1];
				if(len>=low_len&&len<=high_len)
				{
					long long mag=pref_m[j]-pref_m[i-1];
					best[i][j]=max(best[i][j],best[i-1][j-1]+mag);
				}
			}
		}

	cout<<best[n][n]<<'\n';
	return 0;
}
