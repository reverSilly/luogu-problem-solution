//
// 题目：P1806 跑步
// https://www.luogu.com.cn/problem/P1806
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{505};

i64 f[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	f[0][0]=1;
	for(int k=1;k<=n;++k)
	{
		for(int s=0;s<=n;++s)
		{
			f[k][s]=f[k-1][s];
			if(s>=k)
				f[k][s]+=f[k-1][s-k];
		}
	}
	cout<<f[n][n]-1<<'\n';
	return 0;
}
