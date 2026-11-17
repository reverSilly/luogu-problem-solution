// 题目：P2836 [ICPC 1993 WF] 加油问题
// 每站加满 + 出发满油 ⇒ 油量 = C-(x_i-x_j)/M 由"上次加油点 j"唯一确定。
// 状态 (i,j)：到点 i、上次加油在 j 的最小花费(分)。O(n^2) DP。
// 停的合法性：油<半箱 或 到不了下一点(被迫)；停则加满油钱四舍五入到分 +200 分零食。
// 半箱条款是禁令非强制 ⇒ 在所有合法策略里取最小花费。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{60};				// 点数 n+2<=53
constexpr i64 inf{1145141919810};
constexpr double eps{1e-9};

int n;
double D,C,M,R,c0,x[mxn],pr[mxn];
i64 dp[mxn][mxn];					// dp[i][j]=到点 i、上次加油在点 j 的最小花费(分)

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>D>>C>>M>>c0>>n;
	R=C*M;							// 满油续航(英里)
	x[0]=0;x[n+1]=D;
	for(int i=1;i<=n;++i)
		cin>>x[i]>>pr[i];			// pr 单位：分/加仑
	fill_n(&dp[0][0],mxn*mxn,inf);	// 全局默认 0 会被当成可达，必须显式填 inf
	if(x[1]<=R+eps)
		dp[1][0]=0;					// 出发满油，能到第一个点
	for(int i=1;i<=n;++i)
		for(int j=0;j<i;++j)
		{
			if(dp[i][j]>=inf)
				continue;
			double fuel=C-(x[i]-x[j])/M;	// 到 i 时的剩油(加仑)
			if(x[i+1]-x[j]<=R+eps)			// 不停：直接开到下一点
				dp[i+1][j]=min(dp[i+1][j],dp[i][j]);
			bool half=fuel<C/2-eps;			// 不足半箱才允许停
			bool forced=x[i+1]-x[i]>fuel*M+eps;	// 到不了下一点 ⇒ 被迫停
			if((half||forced)&&x[i+1]-x[i]<=R+eps)
			{
				i64 add=(i64)llround((C-fuel)*pr[i]+1e-9)+200;	// 加满的油钱(四舍五入到分)+2 元零食
				dp[i+1][i]=min(dp[i+1][i],dp[i][j]+add);
			}
		}
	i64 best=inf;
	for(int j=0;j<=n;++j)
		best=min(best,dp[n+1][j]);
	i64 tot=best+(i64)llround(c0*100+1e-9);	// 加上出发地加满油的费用
	cout<<tot/100<<'.'<<setw(2)<<setfill('0')<<tot%100<<'\n';
	return 0;
}
