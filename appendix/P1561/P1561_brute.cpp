#include<bits/stdc++.h>
using namespace std;

int main()
{
	int n;
	cin>>n;
	vector<pair<int,int>> c(n);
	for(int i=0;i<n;++i)
		cin>>c[i].first>>c[i].second;
	vector<int> p(n);
	iota(p.begin(),p.end(),0);
	long long best=LLONG_MAX;
	do
	{
		long long su=0,fd=0;
		for(int idx:p)
		{
			su+=c[idx].first;
			fd=max(fd,su)+c[idx].second;
		}
		best=min(best,fd);
	}while(next_permutation(p.begin(),p.end()));
	cout<<best<<'\n';
	return 0;
}
