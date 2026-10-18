//
// 题目：P1545 [USACO04DEC] Dividing the Path G
// https://www.luogu.com.cn/problem/P1545
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxl{1000005};       // L<=10^6 (+5 松弛)
constexpr int inf{0x3f3f3f3f};

int n,L,A,B;
int diff_arr[mxl];                // 差分数组标记奶牛草区严格内部禁止切分的点
int dp[mxl];
int q[mxl];                       // 单调队列维护转移区间 [x-2B, x-2A] 内的偶数下标

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>L))
		return 0;
	cin>>A>>B;
	for(int i=0;i<n;++i)
	{
		int s,e;
		cin>>s>>e;
		if(s+1<=e-1)
		{
			++diff_arr[s+1];
			--diff_arr[e];
		}
	}

	for(int i=1;i<=L;++i)
		diff_arr[i]+=diff_arr[i-1];

	memset(dp,0x3f,sizeof(int)*(L+1));
	dp[0]=0;

	int ql=0,qr=0;
	int cur=0;                    // 待入队的偶数位置指针
	for(int x=2;x<=L;x+=2)
	{
		// 将新进入 [x-2B, x-2A] 右界的 x-2A 加入单调队列
		while(cur<=x-2*A)
		{
			if(dp[cur]<inf)
			{
				while(ql<qr&&dp[q[qr-1]]>=dp[cur])
					--qr;
				q[qr++]=cur;
			}
			cur+=2;
		}
		// 弹出滑出左界 x-2B 的过期下标
		while(ql<qr&&q[ql]<x-2*B)
			++ql;
		// 若当前偶数点不在任何奶牛区间的严格内部, 则可作为分割点
		if(diff_arr[x]==0&&ql<qr)
			dp[x]=dp[q[ql]]+1;
	}

	if(dp[L]>=inf)
		cout<<-1<<'\n';
	else
		cout<<dp[L]<<'\n';
	return 0;
}
