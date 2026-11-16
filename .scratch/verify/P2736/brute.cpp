// P2736 暴力：枚举歌曲子集，按顺序贪心装 CD（装满当前再开新），检查 CD 数<=M
#include<bits/stdc++.h>
using namespace std;
int n,T,m,a[25];
int main()
{
	cin>>n>>T>>m;
	for(int i=0;i<n;++i)cin>>a[i];
	int ans=0;
	for(int mask=0;mask<(1<<n);++mask)
	{
		int cds=0,rem=0,cnt=0;
		bool ok=true;
		for(int i=0;i<n;++i)
		{
			if(!(mask>>i&1))continue;
			++cnt;
			if(a[i]>T){ok=false;break;}
			if(a[i]>rem){++cds;rem=T;}	// 开新 CD
			rem-=a[i];
		}
		if(ok&&cds<=m)ans=max(ans,cnt);
	}
	cout<<ans<<'\n';
	return 0;
}