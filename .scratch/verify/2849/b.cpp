// P2849 暴力：位掩码枚举跳过的中间点集合（恰好 min(k,n-2) 个）
#include<bits/stdc++.h>
using namespace std;
int n,k,x[15],y[15];
int d(int u,int v){return abs(x[u]-x[v])+abs(y[u]-y[v]);}
int main()
{
	cin>>n>>k;
	for(int i=0;i<n;++i)cin>>x[i]>>y[i];
	int need=min(k,n-2),ans=1145141919;
	for(int m=0;m<(1<<(n-2));++m)
	{
		if(__builtin_popcount(m)!=need)continue;
		int prv=0,sum=0;
		for(int i=1;i<n-1;++i)
			if(!(m>>(i-1)&1)){sum+=d(prv,i);prv=i;}
		sum+=d(prv,n-1);
		ans=min(ans,sum);
	}
	cout<<ans<<'\n';
	return 0;
}