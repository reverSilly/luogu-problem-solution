// 题目：P2440 木材加工
// 思路：f(l)=sum(L_i/l) 单调不增，可行 l 构成前缀 ⇒ 二分答案
//       先判 check(1)，不可行输出 0；求最大可行值要偏上界取 mid
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

int n,k,a[mxn];

bool check(int l)
{
	i64 cnt=0;
	for(int i=0;i<n;++i)
	{
		cnt+=a[i]/l;
		if(cnt>=k)return 1;//够 k 段就立刻返回，顺带避免累加过大
	}
	return 0;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	int mx=0;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];
		mx=max(mx,a[i]);
	}
	if(!check(1))//连 1cm 都切不出 k 段
	{
		cout<<0<<'\n';
		return 0;
	}
	int l=1,r=mx;
	while(l<r)
	{
		int mid=(l+r+1)>>1;//偏上界取 mid，否则 l=mid 会死循环
		if(check(mid))l=mid;
		else r=mid-1;
	}
	cout<<l<<'\n';
	return 0;
}
