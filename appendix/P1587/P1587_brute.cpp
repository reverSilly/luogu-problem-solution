#include<bits/stdc++.h>
using namespace std;

int main()
{
	int n,m,k;
	cin>>n>>m>>k;
	long long ans=0;
	for(int y=1;y<=m;++y)
	{
		if(std::gcd(y,k)!=1)
			continue;
		for(int x=1;x<=n;++x)
			if(std::gcd(x,y)==1)
				++ans;
	}
	cout<<ans<<'\n';
	return 0;
}
