// 题目：P2629 好消息，坏消息
// 倍长前缀和 P，旋转 k 合法 <=> min P[k+1..k+n] >= P[k]
// 单调队列滑动窗口最小值，右端 i=k+n，窗口 [i-n+1,i]，O(n)
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1000005};

int n,a[mxn],dq[mxn*2];
i64 P[mxn*2];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];
	P[0]=0;	// 倍长前缀和：P[i]=a 重复两遍后前 i 项之和
	for(int i=1;i<=2*n;++i)
		P[i]=P[i-1]+a[(i-1)%n];
	int head=0,tail=0,ans=0;	// dq[head..tail) 存下标，对应 P 值单调递增
	for(int i=1;i<2*n;++i)
	{
		while(head<tail&&P[dq[tail-1]]>=P[i])--tail;	// 弹队尾：被 i 支配
		dq[tail++]=i;
		while(head<tail&&dq[head]<i-n+1)++head;	// 弹队首：出窗 [i-n+1,i]
		if(i>=n&&P[dq[head]]>=P[i-n])	// 旋转起点 k=i-n
			++ans;
	}
	cout<<ans<<'\n';
	return 0;
}
