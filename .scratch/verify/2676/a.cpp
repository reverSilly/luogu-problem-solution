// 题目：P2676 [USACO07DEC] Bookshelf B
// 思路：降序排序，贪心取最高的牛直到身高和 >= B。
//       最高的 k 头之和 T_k 是 k 头方案的最大和（交换论证）；
//       H_i>=1 ⇒ T_k 严格递增 ⇒ 第一个 T_k>=B 的 k 即最少头数。
//       S<2e9 贴近 int 上限，用 i64 存 B 与 sum 保险。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{20005};

int n,h[mxn];
i64 B,sum;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>B;
	for(int i=0;i<n;++i)
		cin>>h[i];
	sort(h,h+n,greater<>());	// 从高到矮
	int ans=0;
	for(int i=0;i<n&&sum<B;++i)
	{
		sum+=h[i];
		++ans;
	}
	cout<<ans<<'\n';
	return 0;
}
