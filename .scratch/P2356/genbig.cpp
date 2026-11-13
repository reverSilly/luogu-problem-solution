#include<bits/stdc++.h>
using namespace std;

int main()
{
	int n=1000;
	cout<<n<<'\n';
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<n;++j)
			cout<<((i+j)%3==0?0:1000)<<(j==n-1?'\n':' ');
	}
	return 0;
}
