// 题目：P2678 [NOIP 2015 提高组] 跳石头
// 思路：二分最短跳跃距离 x，贪心判定：从起点扫，间距 <x 的岩石移走，否则留下。
//       终点 L 也放进数组一起扫（当作可移走），可以证明这与"终点不可移走"等价：
//       若终点被"移走"，改成移走最后一块留下的岩石即可，前一块留下的岩石距它 >=x。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{50005};

int L,n,m,d[mxn];

bool check(int x)	// 至多移走 m 块，能否让所有相邻间距都 >=x
{
	int cnt=0,last=0;
	for(int i=0;i<=n;++i)	// d[n]=L，终点一起扫
	{
		if(d[i]-last<x)
		{
			if(++cnt>m)
				return false;
		}
		else
			last=d[i];
	}
	return true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>L>>n>>m;
	for(int i=0;i<n;++i)
		cin>>d[i];
	d[n]=L;
	int l=1,r=L;	// 位置互不相同的整数 ⇒ 不移走任何岩石时最短间距 >=1，故答案 >=1
	for(;l<r;)
	{
		int mid=(l+r+1)>>1;	// 偏上界取 mid，找最大可行值；l+r+1<=2e9+1 不溢出
		if(check(mid))
			l=mid;
		else
			r=mid-1;
	}
	cout<<l<<'\n';
	return 0;
}