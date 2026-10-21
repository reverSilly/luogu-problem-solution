//
// 题目：P1869 愚蠢的组合数
// https://www.luogu.com.cn/problem/P1869
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2005};

int c[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	for(int i=0;i<mxn;++i)
	{
		c[i][0]=1;
		for(int j=1;j<=i;++j)
			c[i][j]=c[i-1][j]^c[i-1][j-1];
	}

	int t;
	cin>>t;
	while(t--)
	{
		i64 n,k;
		cin>>n>>k;
		if(n>=mxn)
		{
			// Kummer：2 的幂次等于 popcount(k)+popcount(n-k)-popcount(n)
			unsigned long long un=(unsigned long long)n;
			unsigned long long uk=(unsigned long long)k;
			int v2=__builtin_popcountll(uk)+__builtin_popcountll(un-uk)-__builtin_popcountll(un);
			cout<<(v2==0?1:0)<<'\n';
		}
		else
			cout<<c[(int)n][(int)k]<<'\n';
	}
	return 0;
}
