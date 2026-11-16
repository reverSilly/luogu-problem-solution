// 题目：P2759 奇怪的函数
// x^x 位数 >= n ⟺ x^x >= 10^(n-1) ⟺ x*log10(x) >= n-1
// 左边关于 x 严格递增 ⇒ 二分第一个满足的 x；long double 保精度
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 n;

bool check(i64 x)
{
	return x*log10l((long double)x)>=n-1;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	i64 l=1,r=2'000'000'000;	// check(2e9) 恒真：2e9*log10(2e9)≈1.86e10 >= n-1
	for(;l<r;)
	{
		i64 mid=(l+r)>>1;
		if(check(mid))
			r=mid;
		else
			l=mid+1;
	}
	cout<<l<<'\n';
	return 0;
}