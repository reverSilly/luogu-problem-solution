//
// 题目：P1648 看守
// https://www.luogu.com.cn/problem/P1648
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr i64 inf{1145141919810114514LL};

int n,d;
i64 mx[8],mn[8],x[4];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>d;
	int masks=1<<(d-1);
	for(int m=0;m<masks;++m)
	{
		mx[m]=-inf;
		mn[m]=inf;
	}

	for(int i=0;i<n;++i)
	{
		for(int j=0;j<d;++j)
			cin>>x[j];
		for(int m=0;m<masks;++m)
		{
			i64 s=x[0];
			for(int j=1;j<d;++j)
				s+=(m>>(j-1)&1)?-x[j]:x[j];
			mx[m]=max(mx[m],s);
			mn[m]=min(mn[m],s);
		}
	}

	i64 ans=0;
	for(int m=0;m<masks;++m)
		ans=max(ans,mx[m]-mn[m]);
	cout<<ans<<'\n';
	return 0;
}
