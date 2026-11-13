// 题目：P2255 [USACO14JAN] Recording the Moolympics S
// 思路：按结束时间排序，维护两台调谐器各自的空闲时刻 e[0]<=e[1]
//       能放就放；两机都空闲时更新较大的 e[1]（保留更早空闲的 e[0]）
//       半开区间：s>=end 即相容
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{155};

int n;
struct interval{int s,t;}a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i].s>>a[i].t;
	sort(a,a+n,[](interval x,interval y){return x.t<y.t;});
	int e[2]={0,0},ans=0;
	for(int i=0;i<n;++i)
	{
		int s=a[i].s,t=a[i].t;
		if(s>=e[1])
		{
			// 两台都空闲，用空闲得最晚的那台
			e[1]=t;
			++ans;
		}
		else if(s>=e[0])
		{
			// 只有较早空闲的那台可用
			e[0]=t;
			if(e[0]>e[1])
				swap(e[0],e[1]);
			++ans;
		}
	}
	cout<<ans<<'\n';
	return 0;
}
