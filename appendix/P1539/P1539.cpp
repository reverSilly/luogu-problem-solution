//
// 题目：P1539 [TJOI2011] 01矩阵
// https://www.luogu.com.cn/problem/P1539
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mod{10007};
constexpr int mxn{230};           // n*m<=225 (+5 松弛)
constexpr int mxs{1<<15};         // min(n,m)<=15

int n,m;
char g[mxn][mxn],tmp[mxn][mxn];
int dp[mxs],ndp[mxs];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>m))
		return 0;
	for(int i=0;i<n;++i)
		cin>>g[i];

	// 保证列数 m <= 15
	if(n<m)
	{
		for(int i=0;i<n;++i)
			for(int j=0;j<m;++j)
				tmp[j][i]=g[i][j];
		swap(n,m);
		for(int i=0;i<n;++i)
			for(int j=0;j<m;++j)
				g[i][j]=tmp[i][j];
	}

	int lim=1<<m;
	dp[0]=1;
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
		{
			memset(ndp,0,sizeof(int)*lim);
			char ch=g[i][j];
			for(int mask=0;mask<lim;++mask)
			{
				if(!dp[mask])
					continue;
				int val=dp[mask];
				// 尝试填 0
				if(ch!='1')
				{
					int nmask=mask&~(1<<j);
					ndp[nmask]=(ndp[nmask]+val)%mod;
				}
				// 尝试填 1: 要求上方(mask 第 j 位)与左方(j>0 时 mask 第 j-1 位)均为 0
				if(ch!='0')
				{
					bool up=(mask>>j)&1;
					bool left=(j>0)&&((mask>>(j-1))&1);
					if(!up&&!left)
					{
						int nmask=mask|(1<<j);
						ndp[nmask]=(ndp[nmask]+val)%mod;
					}
				}
			}
			memcpy(dp,ndp,sizeof(int)*lim);
		}

	int ans=0;
	for(int mask=0;mask<lim;++mask)
		ans=(ans+dp[mask])%mod;
	cout<<ans<<'\n';
	return 0;
}
