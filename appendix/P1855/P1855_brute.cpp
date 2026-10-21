//
// 题目：P1855 榨取kkksc03
// https://www.luogu.com.cn/problem/P1855
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25};

int n,mny,tim;
int c[mxn],t[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>mny>>tim;
	for(int i=0;i<n;++i)
		cin>>c[i]>>t[i];
	int ans=0;
	int lim=1<<n;
	for(int s=0;s<lim;++s)
	{
		int sm=0,st=0,cnt=0;
		bool ok=true;
		for(int i=0;i<n;++i)
			if(s>>i&1)
			{
				sm+=c[i];
				st+=t[i];
				++cnt;
				if(sm>mny||st>tim)
				{
					ok=false;
					break;
				}
			}
		if(ok)
			ans=max(ans,cnt);
	}
	cout<<ans<<'\n';
	return 0;
}
