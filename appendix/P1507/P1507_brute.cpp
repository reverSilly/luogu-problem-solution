//
// 题目：P1507 NASA的食物计划
// 暴力基准: 枚举 2^n 个子集,取可行的最大卡路里(仅小 n 可用,绝对正确)
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25};            // 暴力只跑 n<=20

int H,T,n;
int h[mxn],t[mxn],k[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>H>>T>>n;
	for(int i=0;i<n;++i)
		cin>>h[i]>>t[i]>>k[i];

	int ans=0;                    // 一件不选恒可行
	for(int mask=0;mask<(1<<n);++mask)
	{
		int sh=0,st=0,sk=0;
		for(int i=0;i<n;++i)
			if(mask>>i&1)
				sh+=h[i],st+=t[i],sk+=k[i];
		if(sh<=H&&st<=T)
			ans=max(ans,sk);
	}
	cout<<ans<<'\n';
	return 0;
}
