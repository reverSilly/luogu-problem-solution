#include<bits/stdc++.h>
using namespace std;
int main()
{
	static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
	int m=1+rng()%5;
	printf("%d\n",m);
	for(int t=0;t<m;++t)
	{
		int n=20;
		printf("%d",n);
		for(int i=0;i<n;++i)printf(" %d",1+(int)(rng()%3));
		printf("\n");
	}
	return 0;
}
