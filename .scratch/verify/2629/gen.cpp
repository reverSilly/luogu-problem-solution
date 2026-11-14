#include<bits/stdc++.h>
using namespace std;
int main()
{
	mt19937 rn(1);
	int n=1000000;
	printf("%d\n",n);
	for(int i=0;i<n;++i)printf("%d ",(int)(rn()%2001)-1000);
	printf("\n");
	return 0;
}