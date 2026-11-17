// 题目：P3040 [USACO12JAN] Bale Share S
// dp[a][b]=能否凑出农场1重a、农场2重b（农场3=前缀和-a-b）；bitset压第二维
#include<bits/stdc++.h>
using namespace std;

constexpr int mxs{2005};

int n,s[25],sum;
bitset<mxs>dp[mxs];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>s[i],sum+=s[i];
	dp[0][0]=1;
	int cur=0;
	for(int i=0;i<n;++i)
	{
		for(int a=cur;a>=0;--a)	// 倒序：读到的dp[a]仍是"未放第i包"的旧值
		{
			dp[a+s[i]]|=dp[a];		// 第i包给农场1
			dp[a]|=dp[a]<<s[i];		// 第i包给农场2；给农场3=原地保留
		}
		cur+=s[i];
	}
	int ans=sum;
	for(int a=0;a<=sum;++a)
		for(int b=0;b<=sum-a;++b)
			if(dp[a][b])
				ans=min(ans,max({a,b,sum-a-b}));
	cout<<ans<<'\n';
	return 0;
}
