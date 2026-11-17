// 题目：P3078 [USACO13MAR] Poker Hands S
// 顺子 = 区间减 1；最少操作数 = 差分数组正部之和（积木大赛模型）
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

int n,a[mxn];
i64 ans;	// 正差分和最大约 5e9，int 会溢出

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int i=0;i<n;++i)
		ans+=max(0,a[i]-(i?a[i-1]:0));	// a[0] 前虚拟一个 0
	cout<<ans<<'\n';
	return 0;
}
