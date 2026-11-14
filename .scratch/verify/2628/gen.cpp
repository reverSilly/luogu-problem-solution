#include<bits/stdc++.h>
using namespace std;
int main()
{
	mt19937 rn(2);
	int L=1000000;
	string chars=">>**ab f";
	string s;
	for(int i=0;i<L;++i)s+=chars[rn()%chars.size()];
	printf("%s\n",s.c_str());
	int n=100000;
	printf("%d\n",n);
	for(int i=0;i<n;++i)printf("%d ",1+(int)(rn()%6));
	printf("\n");
	return 0;
}