//
// 题目：P1595 信封问题
// https://www.luogu.com.cn/problem/P1595
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{25};

i64 d[mxn];                     // d[i]: i 封信全部装错的方案数

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	d[0]=1;
	d[1]=0;
	for(int i=2;i<=n;++i)
		d[i]=(i-1)*(d[i-1]+d[i-2]);
	cout<<d[n]<<'\n';
	return 0;
}
