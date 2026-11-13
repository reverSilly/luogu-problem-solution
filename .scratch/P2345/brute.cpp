#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	int n;
	cin>>n;
	vector<int>v(n),x(n);
	for(int i=0;i<n;++i)
		cin>>v[i]>>x[i];
	i64 ans=0;
	for(int i=0;i<n;++i)
		for(int j=i+1;j<n;++j)
			ans+=(i64)max(v[i],v[j])*abs(x[i]-x[j]);
	cout<<ans<<'\n';
	return 0;
}
