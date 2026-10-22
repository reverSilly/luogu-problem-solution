// 题目：P2043 质因子分解
// 暴力：把 1..N 每个数单独试除分解，把质因子指数累加
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005};

int cnt[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	for(int i=1;i<=n;++i)
	{
		int x=i;
		for(int p=2;p*p<=x;++p)
		{
			while(x%p==0)
			{
				++cnt[p];
				x/=p;
			}
		}
		if(x>1) ++cnt[x];
	}
	for(int p=2;p<=n;++p)
		if(cnt[p]) cout<<p<<' '<<cnt[p]<<'\n';
	return 0;
}
