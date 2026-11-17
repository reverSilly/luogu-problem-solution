// 题目：P2910 [USACO08OPEN] Clear And Present Danger S
// 必经序列 A1..AM 只是"子序列"约束 ⇒ 答案 = Σ dis[A_i][A_{i+1}]。
// 完全有向图、边权非负，Floyd 求全源最短路；dis 初值就是读入矩阵，无需 INF。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{105},mxm{10005};

int n,m,a[mxm],dis[mxn][mxn];
i64 ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<m;++i)
	{
		cin>>a[i];--a[i];
	}
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>dis[i][j];
	for(int k=0;k<n;++k)
		for(int i=0;i<n;++i)
			for(int j=0;j<n;++j)
				dis[i][j]=min(dis[i][j],dis[i][k]+dis[k][j]);
	for(int i=1;i<m;++i)
		ans+=dis[a[i-1]][a[i]];
	cout<<ans<<'\n';
	return 0;
}
