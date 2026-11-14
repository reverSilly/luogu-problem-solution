#include<bits/stdc++.h>
using namespace std;
int main(int argc,char**argv)
{
	mt19937 rn(atoi(argv[1]));
	int m=1+rn()%4,n=1+rn()%4;
	printf("%d %d\n",m,n);
	for(int i=0;i<m;++i)for(int j=0;j<n;++j)
		printf("%d%c",rn()%21,j+1==n?'\n':' ');
	for(int i=0;i<m;++i)for(int j=0;j<n;++j)
		printf("%d%c",rn()%2,j+1==n?'\n':' ');
	return 0;
}