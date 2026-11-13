#include<bits/stdc++.h>
using namespace std;
using i64=long long;

// O(n^2) 暴力：枚举所有奶牛对
int main()
{
	int n;
	cin>>n;
	vector<i64>d(n);
	i64 S=0;
	for(auto&x:d)
	{
		cin>>x;
		S+=x;
	}
	vector<i64>P(n+1,0);
	for(int i=0;i<n;++i)
		P[i+1]=P[i]+d[i];
	i64 ans=0;
	for(int i=0;i<n;++i)
		for(int j=i+1;j<n;++j)
		{
			i64 arc=P[j]-P[i];
			ans=max(ans,min(arc,S-arc));
		}
	cout<<ans<<'\n';
	return 0;
}
