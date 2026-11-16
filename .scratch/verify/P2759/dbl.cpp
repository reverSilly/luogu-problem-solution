#include<bits/stdc++.h>
using namespace std;
using i64=long long;
int main()
{
	i64 x=137024774;
	long double vl=x*log10l((long double)x);
	double vd=x*log10((double)x);
	i64 k=(i64)roundl(vl);
	printf("x=%lld\nlong double f=%.20Lg\n",x,vl);
	printf("double      f=%.20g\n",vd);
	printf("nearest integer k=%lld\n",k);
	printf("exact-side: f %s k\n",vl<k?"<":">");
	printf("double check(x) for n-1=k: %s (long double: %s)\n",(vd>=k?"true":"false"),(vl>=k?"true":"false"));
	return 0;
}
