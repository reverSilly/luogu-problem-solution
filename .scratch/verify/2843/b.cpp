// P2843 暴力：O(N^2 K) 枚举所有段，逐维数 bit
#include<bits/stdc++.h>
using namespace std;
int n,k,a[105];
int main()
{
	cin>>n>>k;
	for(int i=0;i<n;++i)cin>>a[i];
	int ans=0;
	for(int l=0;l<n;++l)
	{
		int c[35]={};
		for(int r=l;r<n;++r)
		{
			for(int b=0;b<k;++b)c[b]+=(a[r]>>b)&1;
			bool ok=1;
			for(int b=1;b<k;++b)if(c[b]!=c[0])ok=0;
			if(ok)ans=max(ans,r-l+1);
		}
	}
	cout<<ans<<'\n';
	return 0;
}