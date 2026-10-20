//
// P1574 暴力基准：直接筛出 [1,N] 每个数的约数个数,线性扫出 supernumber
// 仅用于 a_i <= 1e6 的小数据对拍。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxv{1000005};

int d[mxv];        // d[i]=i 的约数个数
int last[mxv];     // last[i]=不超过 i 的最大 supernumber

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int m;
	cin>>m;
	vector<i64>a(m);
	i64 mx=0;
	for(int i=0;i<m;++i)
	{
		cin>>a[i];
		mx=max(mx,a[i]);
	}
	int N=(int)mx;
	for(int i=1;i<=N;++i)
		for(int j=i;j<=N;j+=i)
			++d[j];
	int best=0,lastH=0;
	for(int i=1;i<=N;++i)
	{
		if(d[i]>best)
		{
			best=d[i];
			lastH=i;
		}
		last[i]=lastH;
	}
	for(int i=0;i<m;++i)
		cout<<last[(int)a[i]]<<'\n';
	return 0;
}
