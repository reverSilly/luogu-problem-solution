//
// 题目：P1577 切绳子
// https://www.luogu.com.cn/problem/P1577
//
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{10005};        // n<=10000 (+5 松弛)

int n,k;
i64 a[mxn];                      // a[i]=第 i 条绳子的长度,单位:分(原长*100 取整)
i64 mx;                          // 最长绳长(分),二分的上界

// 判定:每根绳子切 d 分长,总共能否得到 k 条
bool ok(i64 d)
{
	i64 cnt=0;
	for(int i=0;i<n;++i)
	{
		cnt+=a[i]/d;
		if(cnt>=k)            // 够了就撤,省下后面的除法
			return true;
	}
	return false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>k;
	for(int i=0;i<n;++i)
	{
		double x;
		cin>>x;
		a[i]=llround(x*100);   // 2 位小数 -> 整数分;总误差 <2e-9,round 后精确
		mx=max(mx,a[i]);
	}
	i64 lo=1,hi=mx,ans=0;      // ans=0:连 1 分都切不出 k 条,答案即 0.00
	while(lo<=hi)
	{
		i64 mid=(lo+hi)>>1;
		if(ok(mid))
		{
			ans=mid;          // mid 可行,记录并向右探更长
			lo=mid+1;
		}
		else
			hi=mid-1;
	}
	cout<<ans/100<<'.';        // 手工拆成 X.XX,不再过浮点
	if(ans%100<10)
		cout<<'0';
	cout<<ans%100<<'\n';
	return 0;
}
