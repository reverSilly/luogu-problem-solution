//
// 题目：P1614 爱与愁的心痛
// https://www.luogu.com.cn/problem/P1614
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{3005};              // n<=3000

int n,m,a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>a[i];
	if(m==0)                          // 长度 0 的区间和为 0
	{
		cout<<0<<'\n';
		return 0;
	}
	i64 cur=0;
	for(int i=0;i<m;++i)
		cur+=a[i];
	i64 ans=cur;
	for(int i=m;i<n;++i)              // 滑动窗口
	{
		cur+=a[i]-a[i-m];
		ans=min(ans,cur);
	}
	cout<<ans<<'\n';
	return 0;
}
