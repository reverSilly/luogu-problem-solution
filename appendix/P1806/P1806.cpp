//
// 题目：P1806 跑步
// https://www.luogu.com.cn/problem/P1806
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{505};

i64 f[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	f[0]=1;
	for(int i=1;i<=n;++i)
		for(int j=n;j>=i;--j)
			f[j]+=f[j-i];
	cout<<f[n]-1<<'\n';
	return 0;
}
