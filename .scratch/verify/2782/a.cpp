// 题目：P2782 友好城市
// 两条航道相交 ⇔ 两岸坐标顺序相反 ⇒ 按南岸排序后，答案就是北岸坐标的严格 LIS。
// N=2e5 ⇒ O(N log N) 贪心 + 二分（tails 数组）。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{200005};

int n,len,tail[mxn];
struct{int s,t;}a[mxn];	// s=南岸坐标，t=北岸坐标

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i].s>>a[i].t;
	sort(a,a+n,[](auto x,auto y){return x.s<y.s;});
	for(int i=0;i<n;++i)
	{
		int p=lower_bound(tail,tail+len,a[i].t)-tail;	// 严格递增 ⇒ lower_bound
		tail[p]=a[i].t;
		if(p==len)
			++len;
	}
	cout<<len<<'\n';
	return 0;
}
