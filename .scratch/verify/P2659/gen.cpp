#include<bits/stdc++.h>
using namespace std;
int main(int argc,char**argv)
{
	mt19937 rn(atoi(argv[1]));
	int n=1+rn()%60;
	printf("%d\n",n);
	for(int i=0;i<n;++i)
		printf("%d%c",rn()%11,i+1==n?'\n':' ');
	return 0;
}