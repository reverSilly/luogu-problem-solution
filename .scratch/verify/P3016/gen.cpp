#include<bits/stdc++.h>
using namespace std;
int main(int argc,char**argv)
{
	int seed=atoi(argv[1]);
	if(seed==0)	// 极限规模：n=700,k=20，全值域
	{
		int n=700,k=20;
		printf("%d %d\n",n,k);
		unsigned s=12345;
		for(int i=1;i<=n;++i)
			for(int j=1;j<=i;++j)
			{
				s=s*1103515245u+12345u;
				long long v=(long long)(s%2000000001u)-1000000000;
				printf("%lld%c",v,j==i?'\n':' ');
			}
		return 0;
	}
	srand(seed);
	int n=rand()%8+1,k=rand()%n+1;
	printf("%d %d\n",n,k);
	for(int i=1;i<=n;++i)
		for(int j=1;j<=i;++j)
			printf("%d%c",rand()%41-20,j==i?'\n':' ');
	return 0;
}