//
// 题目：P1626 象棋比赛
// https://www.luogu.com.cn/problem/P1626
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

int n,k,a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	for(int i=0;i<n;++i)
		cin>>a[i];
	sort(a,a+n);
	// 任意若干条相邻差都能同时取到：每个选手最多占左右两条边各一次（左白右黑）
	static int d[mxn];
	for(int i=0;i+1<n;++i)
		d[i]=a[i+1]-a[i];
	sort(d,d+n-1);
	i64 ans=0;
	for(int i=0;i<k;++i)
		ans+=d[i];
	cout<<ans<<'\n';
	return 0;
}
