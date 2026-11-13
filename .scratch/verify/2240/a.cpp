// 题目：P2240 【深基12.例1】部分背包问题
// 思路：按单位价值 v/m 降序，能整堆拿就整堆拿，最后一堆按剩余容量取一部分
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int n,T;
struct pile{int m,v;};
pile a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>T;
	for(int i=0;i<n;++i)
		cin>>a[i].m>>a[i].v;
	// 交叉相乘比较 v/m，避免浮点误差
	sort(a,a+n,[](auto x,auto y){return 1LL*x.v*y.m>1LL*y.v*x.m;});

	double ans=0;
	int rem=T;
	for(int i=0;i<n;++i)
	{
		if(a[i].m<=rem)
		{
			ans+=a[i].v;
			rem-=a[i].m;
		}
		else
		{
			// 注意先转 double，否则是整数除法
			ans+=(double)rem*a[i].v/a[i].m;
			rem=0;
			break;
		}
	}
	cout<<fixed<<setprecision(2)<<ans<<'\n';
	return 0;
}
