//
// 题目：P1620 漂亮字串
// https://www.luogu.com.cn/problem/P1620
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 co,cx,mo,mx;

i64 ceildiv(i64 x,i64 y)           // 0 个字母需要 0 段
{
	if(x<=0)
		return 0;
	return (x+y-1)/y;
}

bool check(i64 s)                  // 长度 s 是否可行：枚举 O 的个数 o，X 的个数 s-o
{
	if(s==0)
		return true;
	i64 lo=max<i64>(0,s-cx),hi=min<i64>(s,co);
	if(lo>hi)
		return false;
	// 条件一 ceil(o/mo)<=x+1 随 o 单调变松，其解集是一个后缀
	i64 l=lo,r=hi;
	while(l<r)
	{
		i64 mid=(l+r+1)>>1;
		if(ceildiv(mid,mo)<=s-mid+1)
			l=mid;
		else
			r=mid-1;
	}
	i64 o=l,x=s-l;
	if(ceildiv(o,mo)>x+1)          // 连最松的 o 都不满足条件一
		return false;
	return ceildiv(x,mx)<=o+1;     // 条件二在满足条件一的 o 中取最靠右者最有利
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	while(cin>>co>>cx>>mo>>mx)
	{
		if(mo==0||mx==0)           // 该字母一次都不能出现
		{
			if(mo==0&&mx==0)
				cout<<0<<'\n';
			else if(mo==0)
				cout<<min(cx,mx)<<'\n';
			else
				cout<<min(co,mo)<<'\n';
			continue;
		}
		i64 l=0,r=co+cx;
		while(l<r)
		{
			i64 mid=(l+r+1)>>1;
			if(check(mid))
				l=mid;
			else
				r=mid-1;
		}
		cout<<l<<'\n';
	}
	return 0;
}
