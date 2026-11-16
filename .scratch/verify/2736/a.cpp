// 题目：P2736 [USACO3.4] “破锣摇滚”乐队 Raucous Rockers
// dp[i][j][t]=考虑完前 i 首歌、已开 j 张 CD、当前 CD 还剩 t 分钟时，最多选了几首。
// j=0 表示还没开 CD，此时 t=0（没有「当前 CD」可塞）。
// 每首歌三选一：跳过 / 塞进当前 CD（j>0 且 a[i]<=t）/ 新开一张 CD（j<M 且 a[i]<=T）。
// 顺序约束由「按 i 递增决策、CD 只前进不回头」自动保证。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25},inf{1145141919};

int n,T,m,a[mxn],dp[mxn][mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>T>>m;
	for(int i=0;i<n;++i)
		cin>>a[i];
	fill_n(&dp[0][0][0],mxn*mxn*mxn,-inf);	// 不可达状态必须显式填 -INF
	dp[0][0][0]=0;	// 还没开 CD：j=0,t=0
	for(int i=0;i<n;++i)
		for(int j=0;j<=m;++j)
			for(int t=0;t<=T;++t)
			{
				if(dp[i][j][t]<0)
					continue;
				int cur=dp[i][j][t];
				dp[i+1][j][t]=max(dp[i+1][j][t],cur);	// 跳过第 i 首
				if(j>0&&a[i]<=t)	// 放进当前 CD（必须已有 CD）
					dp[i+1][j][t-a[i]]=max(dp[i+1][j][t-a[i]],cur+1);
				if(j<m&&a[i]<=T)	// 新开一张 CD
					dp[i+1][j+1][T-a[i]]=max(dp[i+1][j+1][T-a[i]],cur+1);
			}
	int ans=0;
	for(int j=0;j<=m;++j)
		for(int t=0;t<=T;++t)
			ans=max(ans,dp[n][j][t]);
	cout<<ans<<'\n';
	return 0;
}
