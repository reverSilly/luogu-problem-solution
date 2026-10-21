//
// P1614 暴力（对拍用）：枚举每个起点, 直接累加 m 个刺痛值
#include<bits/stdc++.h>
using namespace std;

int n,m,a[3005];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>a[i];
	int ans=1<<30;
	for(int i=0;i+m<=n;++i)
	{
		int s=0;
		for(int j=0;j<m;++j)
			s+=a[i+j];
		ans=min(ans,s);
	}
	if(m==0)
		ans=0;
	cout<<ans<<'\n';
	return 0;
}
