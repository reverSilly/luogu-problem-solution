// 题目：P2842 纸币问题 1
// 完全背包最值形式：dp[j]=凑出 j 的最少张数
// 外层面额、内层金额升序 ⇒ dp[j-a[i]] 已含本轮更新 ⇒ 同种纸币可无限用
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005},mxw{10005};
constexpr int inf{1145141919};

int n,w,a[mxn],dp[mxw];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>w;
	for(int i=0;i<n;++i)
		cin>>a[i];
	fill_n(dp,w+1,inf);	// 全局默认 0 是合法张数！必须显式填 INF
	dp[0]=0;
	for(int i=0;i<n;++i)
		for(int j=a[i];j<=w;++j)	// 升序=完全背包；a[i]>w 时循环为空
			dp[j]=min(dp[j],dp[j-a[i]]+1);	// inf+1 不溢出，min 不会选中它
	cout<<dp[w]<<'\n';
	return 0;
}
