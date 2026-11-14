// 题目：P2759 奇怪的函数
// x^x 至少 n 位 <=> x^x >= 10^(n-1) <=> x*log10(x) >= n-1
// g(x)=x*log10(x) 单调增 ⇒ 二分最小 x；long double + eps 处理 10 的幂处的精确整数边界
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 n;

bool ok(i64 x)
{
	long double g=(long double)x*log10l((long double)x);
	return g>=(long double)(n-1)-1e-7L;	// eps 远大于浮点误差(~1e-9)，远小于相邻 g 间隔(~0.6)
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	i64 l=1,r=1000000000;	// x=1e9 时 g=9e9 >= 2e9-1，必可达
	while(l<r)
	{
		i64 mid=(l+r)>>1;
		if(ok(mid))r=mid;
		else l=mid+1;
	}
	cout<<l<<'\n';
	return 0;
}