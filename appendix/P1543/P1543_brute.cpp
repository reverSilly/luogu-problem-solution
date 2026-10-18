#include<bits/stdc++.h>
using namespace std;

int n;
int a[25];

int main()
{
	cin>>n;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];
		--a[i];
	}
	int best=0;
	for(int mask=0;mask<(1<<n);++mask)
	{
		int cnt=__builtin_popcount(mask);
		if(cnt<=best)
			continue;
		int watched_by_unselected=0;
		for(int i=0;i<n;++i)
			if(!((mask>>i)&1))
				watched_by_unselected|=(1<<a[i]);
		if((mask&watched_by_unselected)==mask)
			best=cnt;
	}
	cout<<best<<'\n';
	return 0;
}
