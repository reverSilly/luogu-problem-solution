// 题目：P2066 机器分配
// 思路：后缀型分组背包，f[i][j]: 公司 i..n 最多使用 j 台设备时的最大盈利
//       还原时从公司 1 开始贪心取最小的 k，保证编号小的公司分得设备尽可能少
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{12};
constexpr int mxm{17};

int n,m;
i64 v[mxn][mxm];			// v[i][j]: 第 i 个公司分 j 台设备的盈利
i64 f[mxn][mxm];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=1;i<=n;++i)
		for(int j=1;j<=m;++j) cin>>v[i][j];
	// 没有公司时盈利为 0（设备可以剩下不用）
	for(int j=0;j<=m;++j) f[n+1][j]=0;
	for(int i=n;i>=1;--i)
	{
		f[i][0]=0;
		for(int j=1;j<=m;++j)
		{
			f[i][j]=0;
			for(int k=0;k<=j;++k)
				f[i][j]=max(f[i][j],v[i][k]+f[i+1][j-k]);
		}
	}
	cout<<f[1][m]<<'\n';
	int rest=m;
	for(int i=1;i<=n;++i)
	{
		int k=0;
		// 取最小的 k，使后缀仍能达到最优值
		while(k<rest&&v[i][k]+f[i+1][rest-k]!=f[i][rest]) ++k;
		cout<<i<<' '<<k<<'\n';
		rest-=k;
	}
	return 0;
}
