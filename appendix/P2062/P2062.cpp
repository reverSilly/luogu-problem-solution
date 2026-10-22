// 题目：P2062 分队问题
// 思路：把 a 升序排序后，答案等于「把序列切成若干连续段，每段长度 >= 该段最大值」的最多段数
//       dp[r] = 1 + max{dp[j] | 0 <= j <= r - b[r]}，用前缀最大值优化到 O(n)
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1000005};

int a[mxn],b[mxn];
int dp[mxn],pre[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	for(int i=1;i<=n;++i) cin>>a[i];
	sort(a+1,a+n+1);
	for(int i=1;i<=n;++i) b[i]=a[i];
	dp[0]=0;
	pre[0]=0;
	for(int r=1;r<=n;++r)
	{
		int t=r-b[r];			// 最后一段取 [j+1,r]，要求 r-j >= b[r]，即 j <= t
		if(t>=0) dp[r]=pre[t]+1;
		else dp[r]=-1;			// 前 r 个人无法分成合法队伍
		pre[r]=max(pre[r-1],dp[r]);
	}
	cout<<dp[n]<<'\n';
	return 0;
}
