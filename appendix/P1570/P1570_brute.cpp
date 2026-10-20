// 暴力基准:枚举所有恰好 m 项的选取,用交叉相乘精确比较比值取最大
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{205};       // 暴力仅跑 n<=20

int n,m;
i64 v[mxn],c[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>v[i];
	for(int i=0;i<n;++i)
		cin>>c[i];

	i64 bp=0,bq=1;            // 当前最大比值 bp/bq
	for(int mask=0;mask<(1<<n);++mask)
	{
		if(__builtin_popcount(mask)!=m)
			continue;
		i64 p=0,q=0;
		for(int j=0;j<n;++j)
			if(mask>>j&1)
			{
				p+=v[j];
				q+=c[j];
			}
		if(p*bq>bp*q)         // p/q > bp/bq (q,bq>0)
		{
			bp=p;
			bq=q;
		}
	}
	cout<<fixed<<setprecision(3)<<(double)bp/bq<<'\n';
	return 0;
}
