// 题目：P2803 学校选址 II（经典邮局问题）
// 直线上最近学校划分必为连续段 ⇒ 划分 DP；段费用 = 加权中位数费用
// w 用前缀和 + 单调中位数指针 O(n^2)；f 必须显式填 INF（默认 0 是合法费用！）
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{105};
constexpr i64 inf{1145141919810};

int n,k;
i64 a[mxn],x[mxn],pa[mxn],px[mxn],w[mxn][mxn],f[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int i=1,d;i<n;++i)
	{
		cin>>d;
		x[i]=x[i-1]+d;
	}
	k=min(k,n);
	for(int i=0;i<n;++i)
	{
		pa[i+1]=pa[i]+a[i];
		px[i+1]=px[i]+a[i]*x[i];
	}
	for(int i=0;i<n;++i)
	{
		int m=i;
		for(int j=i;j<n;++j)
		{
			i64 W=pa[j+1]-pa[i];
			while(2*(pa[m+1]-pa[i])<W)
				++m;	// 加权中位数：左半权重首次 >= 总权重一半；m 随 j 单调右移
			w[i][j]=x[m]*(pa[m+1]-pa[i])-(px[m+1]-px[i])
				+(px[j+1]-px[m+1])-x[m]*(pa[j+1]-pa[m+1]);
		}
	}
	fill_n(&f[0][0],mxn*mxn,inf);	// 0 所学校覆盖不了任何楼，必须 INF 而非默认 0
	f[0][0]=0;
	for(int j=1;j<=k;++j)
		for(int i=j;i<=n;++i)
			for(int t=j-1;t<i;++t)
				f[j][i]=min(f[j][i],f[j-1][t]+w[t][i-1]);
	cout<<f[k][n]<<'\n';
	return 0;
}
