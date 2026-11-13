// 题目：P2389 电脑班的裁员
// 思路：至多 k 段不相交连续区间的最大和（0 段合法 ⇒ 答案非负）
//       f[i][j]：前 i 人、至多 j 段、第 i 人必选且为最后一段右端点
//       g[i][j]：前 i 人、至多 j 段（第 i 人可选可不选）
//       f[i][j]=max(f[i-1][j],g[i-1][j-1])+a[i]   接上一段 / 另起一段
//       g[i][j]=max(g[i-1][j],f[i][j])            第 i 人不选 / 选
//       等价于前缀和 DP 把 max_{t<i}(g[t][j-1]-s[t]) 增量维护，O(nk)
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{505},inf{1145141919};

int n,k,a[mxn],f[mxn][mxn],g[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	for(int i=1;i<=n;++i)
		cin>>a[i];

	// g 的边界全为 0（空选合法），全局默认值正好；f 的不可能状态必须显式 -inf
	for(int j=0;j<=k;++j)
		f[0][j]=-inf;
	for(int i=1;i<=n;++i)
	{
		f[i][0]=-inf;
		for(int j=1;j<=k;++j)
		{
			f[i][j]=max(f[i-1][j],g[i-1][j-1])+a[i];
			g[i][j]=max(g[i-1][j],f[i][j]);
		}
	}
	cout<<g[n][k]<<'\n';
	return 0;
}
