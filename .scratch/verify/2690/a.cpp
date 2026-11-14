// 题目：P2690 [USACO04NOV] Apple Catching G
// 只有两棵树 + 起点在 1 号树 ⇒ 位置 = 移动次数的奇偶（偶 1 奇 2），状态只需 (分钟, 移动数)
// 同一间隙移动两次抵消 ⇒ WLOG 每间隙至多动一次，转移只有"动/不动"两条
// dp[i][j] = 前 i 分钟恰好用 j 次移动的最大收获；答案 max_{j<=w} dp[t][j]
#include<bits/stdc++.h>
using namespace std;

constexpr int mxt{1005},mxw{35};
constexpr int inf{1145141919};

int t,w,a[mxt],dp[mxt][mxw],ans;

void upd(int&x,int v){x=max(x,v);}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>t>>w;
	for(int i=0;i<t;++i)
		cin>>a[i];
	fill_n(&dp[0][0],mxt*mxw,-inf);	// 全局默认 0 是合法收获值，必须显式填 -inf
	dp[0][0]=0;	// 第 1 分钟之前没有间隙，只能用 0 次移动
	for(int i=1;i<=t;++i)
		for(int j=0;j<=w;++j)
		{
			int b=dp[i-1][j];	// 第 i 分钟前不动
			if(j)
				upd(b,dp[i-1][j-1]);	// 第 i 分钟前动一次
			dp[i][j]=b+(a[i-1]==(j&1?2:1));	// 累计 j 次 ⇒ 偶在 1 号树、奇在 2 号树
		}
	for(int j=0;j<=w;++j)
		upd(ans,dp[t][j]);	// "恰好 j 次"不单调，必须对 j 取 max
	cout<<ans<<'\n';
	return 0;
}
