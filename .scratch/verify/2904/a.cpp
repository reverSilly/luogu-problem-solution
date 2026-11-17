// 题目：P2904 [USACO08MAR] River Crossing S
// 最优方案：去程每批≥1头、回程必空 ⇒ 方案= N 的一个有序拆分，代价 Σ(M+pre[k])+(批数-1)*M
// g[i]=送 i 头过河且 FJ 回到近岸的最短时间（含最后一次空返）；g[i]=min g[i-k]+pre[k]+2M
// 答案 g[n]-m：扣掉最后一次不需要的空返
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2505};
constexpr i64 inf{1145141919810};

int n,m,a[mxn];
i64 pre[mxn],g[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];
		pre[i+1]=pre[i]+a[i];	// pre[k]=带 k 头牛比独自划多花的时间
	}
	fill_n(g+1,n,inf);	// g[0]=0；其余必须先填 INF，全局默认 0 会错
	for(int i=1;i<=n;++i)
		for(int k=1;k<=i;++k)
			g[i]=min(g[i],g[i-k]+pre[k]+2LL*m);
	cout<<g[n]-m<<'\n';
	return 0;
}
