//
// 题目：P1589 [USACO05OPEN] 泥泞路
// https://www.luogu.com.cn/problem/P1589
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{10005};

struct seg{int s,e;};
seg a[mxn];
int n,L;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>L;
	for(int i=0;i<n;++i)
		cin>>a[i].s>>a[i].e;
	sort(a,a+n,[](auto x,auto y){return x.s<y.s;});

	i64 ans=0,cur=0;            // cur: 已被木板覆盖到的位置(右开)
	for(int i=0;i<n;++i)
	{
		if(a[i].e<=cur)
			continue;           // 整段已被前面的木板免费覆盖
		i64 st=max<i64>(a[i].s,cur);
		i64 need=(a[i].e-st+L-1)/L;
		ans+=need;
		cur=st+need*L;
	}
	cout<<ans<<'\n';
	return 0;
}
