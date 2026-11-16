// 造极限数据：N=Q=8000，同心嵌套圆（合法 laminar），随机询问点
#include<bits/stdc++.h>
using namespace std;
int main()
{
	mt19937 rn(123);
	int n=8000,q=8000;
	printf("%d\n",n);
	for(int i=1;i<=n;++i)
		printf("0 0 %d\n",i*10000);	// 半径最大 8e7 < 1e8
	printf("%d\n",q);
	for(int i=0;i<q;++i)
		printf("%d %d %d %d\n",(int)(rn()%200000001)-100000000,(int)(rn()%200000001)-100000000,(int)(rn()%200000001)-100000000,(int)(rn()%200000001)-100000000);
	return 0;
}