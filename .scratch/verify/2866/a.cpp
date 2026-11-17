// 题目：P2866 [USACO06NOV] Bad Hair Day S
// 思路：C_i = nge(i)-i-1（右边第一个 >= h_i 的位置）。
//       单调栈从左往右扫，出栈瞬间结算：被弹出的 j 的第一个阻挡者就是当前 i。
//       等高也挡视线 ⇒ 弹出条件是 <=；答案可达 n(n-1)/2 ≈ 3.2e9 ⇒ i64。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{80005};

int n,h[mxn],stk[mxn],top;
i64 ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>h[i];
	for(int i=0;i<n;++i)
	{
		while(top&&h[stk[top-1]]<=h[i])	// 等高也挡：<= 的等号不能丢
		{
			int j=stk[--top];
			ans+=i-j-1;	// j 的可见数在出栈瞬间定格
		}
		stk[top++]=i;
	}
	while(top)	// 右边再无阻挡者：看到队尾
	{
		int j=stk[--top];
		ans+=n-j-1;
	}
	cout<<ans<<'\n';
	return 0;
}
