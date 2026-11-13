#include<bits/stdc++.h>
using namespace std;

int main()
{
	int n=1000,k=100000,m=80,p=1000;
	cout<<n<<' '<<k<<' '<<m<<' '<<p<<'\n';
	string suit="CHSD",rank="A23456789TJQK";
	for(int i=0;i<k;++i)
		cout<<suit[i%4]<<rank[i%13]<<" K\n";
	return 0;
}
