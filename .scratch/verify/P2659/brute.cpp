// P2659 暴力：枚举所有区间，len*min 取最大
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
int n,a[2005];
int main()
{
	cin>>n;
	for(int i=0;i<n;++i)cin>>a[i];
	i64 ans=0;
	for(int l=0;l<n;++l)
	{
		int mn=a[l];
		for(int r=l;r<n;++r)
		{
			mn=min(mn,a[r]);
			ans=max(ans,(i64)mn*(r-l+1));
		}
	}
	cout<<ans<<'\n';
	return 0;
}