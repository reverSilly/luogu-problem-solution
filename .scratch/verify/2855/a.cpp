// 题目：P2855 [USACO06DEC] River Hopscotch S
// 思路：最大化最小值 ⇒ 二分答案 d；判定用贪心「能留就留」：
//       与上一块保留石距离 <d 就移走，否则保留。左端点归纳可证移除数最少。
//       末段 L-lst<d 时补移最后一块保留石（合并后必 >=d），恰 +1 次。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{50005};

int L,n,m,d[mxn];

bool check(int x)	// 全间距 >=x 的最少移除数是否 <=m
{
	int lst=0,cnt=0;	// lst=上一块保留石头的位置，起点锚 0
	for(int i=0;i<n;++i)
		if(d[i]-lst<x)
			++cnt;	// 太近，移走
		else
			lst=d[i];	// 能留就留：保留位置最靠左，给后面留最大余地
	if(L-lst<x)
		++cnt;	// 末段不够：移走最后一块保留石（x<=L 保证 lst>0，有石可移）
	return cnt<=m;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>L>>n>>m;
	for(int i=0;i<n;++i)
		cin>>d[i];
	sort(d,d+n);
	int l=1,r=L;	// 答案 ∈[1,L]；N=0 时恒可行，自动得 L
	for(;l<r;)
	{
		int mid=(l+r+1)>>1;	// 求最大可行值，偏上界取 mid
		if(check(mid))
			l=mid;
		else
			r=mid-1;
	}
	cout<<l<<'\n';
	return 0;
}
