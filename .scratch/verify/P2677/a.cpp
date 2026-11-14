// 题目：P2677 [USACO07DEC] Bookshelf 2 B
// 思路：塔高 = 某个非空子集的身高和；求 >=B 的最小子集和，答案 = 该和 - B。
//       N<=20 ⇒ 直接递推出全部 2^N 个子集和：sum[s]=sum[s^lowbit(s)]+h[ctz(s)]。
//       B<=S 保证全取可行，答案一定存在；且答案 < 塔中最矮奶牛的身高 <= maxH-1。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25},mxs{(1<<20)+5};

int n,b,h[mxn],sum[mxs];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>b;
	for(int i=0;i<n;++i)
		cin>>h[i];
	int ans=1145141919;
	for(int s=1;s<(1<<n);++s)	// s=0 是空集，身高和 0<B，不用考虑
	{
		int low=s&-s;
		sum[s]=sum[s^low]+h[__builtin_ctz(low)];	// 去掉最低位的 1，子问题已算过
		if(sum[s]>=b)
			ans=min(ans,sum[s]-b);
	}
	cout<<ans<<'\n';
	return 0;
}