// 题目：P2032 扫描
// 单调队列维护滑动窗口最大值
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{2000005};

int n,k;
int a[mxn];
// 队列存下标，q[head..tail) 内的元素值单调递减，队首即窗口最大值
int q[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	for(int i=0;i<n;++i) cin>>a[i];

	int head=0,tail=0;
	for(int i=0;i<n;++i)
	{
		// 队首下标滑出窗口
		if(head<tail && q[head]<=i-k) ++head;
		// 队尾比新元素小的都不可能再成为最大值
		while(head<tail && a[q[tail-1]]<=a[i]) --tail;
		q[tail++]=i;
		if(i>=k-1) cout<<a[q[head]]<<'\n';
	}
	return 0;
}
