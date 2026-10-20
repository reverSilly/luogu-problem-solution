//
// 题目：P1567 统计天数
// https://www.luogu.com.cn/problem/P1567
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1000005};       // N<=1e6 (+5 松弛)

int n,a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];
	int ans=1,cnt=1;              // 单天也算长度 1 的上升段
	for(int i=1;i<n;++i)
	{
		if(a[i]>a[i-1])
			++cnt;
		else
			cnt=1;
		ans=max(ans,cnt);
	}
	cout<<ans<<'\n';
	return 0;
}
