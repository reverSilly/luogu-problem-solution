#include<bits/stdc++.h>
using namespace std;
int main(int argc,char**argv)
{
	mt19937 rn(atoi(argv[1]));
	int a=1+rn()%20000,b=a+rn()%(20001-a);
	printf("%d %d\n",a,b);
	return 0;
}