// P2759 精度边际扫描（一次性，放 scratch）：
// 对所有可能成为答案/被二分探测的 x（<= n=2e9 时的答案 238723448 + 2），
// 扫描 |x*log10(x) - 最近整数| 的最小值（跳过 x=10^k：那里取值恰为整数且 long double 精确）。
// 若最小边际 >> long double 误差界(~5e-10)，则 check 的浮点比较对所有 n<=2e9 都与精确比较一致。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	long double v1=238723448LL*log10l(238723448.0L);
	long double v0=238723447LL*log10l(238723447.0L);
	printf("n=2e9: f(ans)-(n-1)=%.6Lg, (n-1)-f(ans-1)=%.6Lg\n",v1-1999999999.0L,1999999999.0L-v0);
	long double best=1;
	i64 bx=0;
	for(i64 x=2;x<=238723450;++x)
	{
		if(x==10||x==100||x==1000||x==10000||x==100000||x==1000000||x==10000000||x==100000000)
			continue;
		long double v=x*log10l((long double)x);
		long double d=fabsl(v-roundl(v));
		if(d<best)
		{
			best=d;bx=x;
		}
	}
	printf("min |x*log10(x)-nearest integer| = %.6Lg at x=%lld\n",best,(long long)bx);
	return 0;
}