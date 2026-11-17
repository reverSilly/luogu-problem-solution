// 题目：P2884 [USACO07MAR] Monthly Expense S
// 二分答案 S∈[max a_i, sum a_i]；check：贪心从左到右尽量塞满一段，段数 ≤ m 即可行。
// a_i>0 保证"子段和 ≤ 整段和"，于是"≤m 段"与"恰好 m 段"等价（m≤n 可继续细分）。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

int n,m,a[mxn];

bool check(i64 s)	// 前提 s>=max a_i；返回最少段数是否 <=m
{
	int cnt=1;
	i64 cur=0;
	for(int i=0;i<n;++i)
	{
		if(cur+a[i]>s)
		{
			++cnt;
			cur=a[i];
			if(cnt>m)
				return false;
		}
		else
			cur+=a[i];
	}
	return true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	i64 l=0,r=0;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];
		l=max(l,(i64)a[i]);	// 下界：含最大元素的那段至少这么大
		r+=a[i];			// 上界：一整段装下全部
	}
	while(l<r)
	{
		i64 mid=(l+r)>>1;
		if(check(mid))
			r=mid;
		else
			l=mid+1;
	}
	cout<<l<<'\n';
	return 0;
}
