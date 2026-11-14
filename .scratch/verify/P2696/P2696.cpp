#include<bits/stdc++.h>
using namespace std;

int n;

int jos(int k)
{
	int m=1;
	while(m*2<=k)
		m*=2;
	return 2*(k-m)+1;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	int k=n;
	while((k&(k+1))!=0)
		k=jos(k);
	cout<<n+k<<'\n';
	return 0;
}