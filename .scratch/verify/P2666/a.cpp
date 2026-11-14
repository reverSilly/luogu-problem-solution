// 题目：P2666 [USACO07OCT] Bessie's Secret Pasture S
// 思路：数有序四元组 (a,b,c,d)≥0 使 a²+b²+c²+d²=N（样例 N=4→5 说明计顺序）。
//       2+2 折半：cnt[s]=平方和为 s 的有序非负整数对数，
//       答案 = Σ cnt[s]·cnt[N-s]。边长 ≤100，总复杂度 O(N)。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{10005};

int n,cnt[mxn];
i64 ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int a=0;a*a<=n;++a)
		for(int b=0;a*a+b*b<=n;++b)
			++cnt[a*a+b*b];			// 每个有序对 (a,b) 恰好计一次
	for(int s=0;s<=n;++s)
		ans+=1ll*cnt[s]*cnt[n-s];	// 前两块面积和 s，后两块面积和 n-s
	cout<<ans<<'\n';
	return 0;
}