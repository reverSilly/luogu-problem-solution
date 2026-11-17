// 题目：P3030 [USACO11NOV] Tile Exchanging S
// 每块砖独立选最终边长 b（0=退掉），面积 b*b、代价 (a-b)^2 ⇒ 分组背包
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{15},mxm{10005},inf{1145141919};

int n,m,a[mxn],f[mxn][mxm];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>a[i];
	fill_n(&f[0][0],mxn*mxm,inf);	// 全局默认0不是INF！不可达必须显式填
	f[0][0]=0;
	for(int i=0;i<n;++i)
		for(int j=0;j<=m;++j)
		{
			if(f[i][j]==inf)
				continue;
			for(int b=0;b<=100&&b*b<=m-j;++b)	// b=0表示退掉这块砖
				f[i+1][j+b*b]=min(f[i+1][j+b*b],f[i][j]+(a[i]-b)*(a[i]-b));
		}
	cout<<(f[n][m]==inf?-1:f[n][m])<<'\n';
	return 0;
}
