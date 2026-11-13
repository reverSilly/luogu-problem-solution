// 题目：P2251 质量检测
// 思路：单调队列维护每个窗口的后缀严格最小值
//       先弹过期队头，再弹被 a[i] 支配的队尾，压入 i
//       队头就是当前窗口的最小值；每个下标进出各一次，O(N)
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

int n,m,a[mxn],Q[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>a[i];
	int hd=0,tl=0;
	for(int i=0;i<n;++i)
	{
		// 先让滑出窗口 [i-m+1,i] 的索引出队
		while(hd<tl&&Q[hd]<=i-m)
			++hd;
		// 队尾值 >=a[i] 的索引被 a[i] 支配，永远不可能再当最小值
		while(hd<tl&&a[Q[tl-1]]>=a[i])
			--tl;
		Q[tl++]=i;
		if(i>=m-1)
			cout<<a[Q[hd]]<<'\n';
	}
	return 0;
}
