// 题目：P2920 [USACO08NOV] Time Management S
// 空闲无益（左移论证）⇒ 连续执行；EDF（截止升序）最优（相邻交换论证）。
// 可行 ⇔ 每个前缀 s+A_k ≤ S_k ⇒ 最晚开始 = min(S_k-A_k)，负则 -1。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005},inf{1145141919};

int n,ans{inf};
struct job{int t,s;}a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i].t>>a[i].s;
	sort(a,a+n,[](job x,job y){return x.s<y.s;});	// EDF：截止早的先做
	int cur=0;
	for(int i=0;i<n;++i)
	{
		cur+=a[i].t;
		ans=min(ans,a[i].s-cur);	// 前缀约束：s ≤ S_i - 前缀耗时
	}
	cout<<(ans<0?-1:ans)<<'\n';	// 负余量 = 连 0 时刻开始都不可行，不能 clamp 成 0
	return 0;
}
