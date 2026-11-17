// 题目：P2940 [USACO09FEB] The Leprechaun S
// 环面网格：行/列/两条对角线共 4N 条环状线，每条线求非空环形最大子段和
// 环形最大子段和 = max(普通 Kadane, 总和-最小子段和)，全负时只能取前者
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{205};
constexpr i64 inf{1145141919810};

int n,a[mxn][mxn],tmp[mxn];
i64 ans=-inf;

i64 circ()	// tmp[0..n) 的非空环形最大子段和
{
	i64 tot=0,mx=-inf,mn=inf,cmx=0,cmn=0;
	for(int i=0;i<n;++i)
	{
		i64 x=tmp[i];
		tot+=x;
		cmx=max(x,cmx+x);mx=max(mx,cmx);
		cmn=min(x,cmn+x);mn=min(mn,cmn);
	}
	if(mx<0)
		return mx;	// 全负：tot-mn=0 对应空段，不合法
	return max(mx,tot-mn);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>a[i][j];
	for(int i=0;i<n;++i)	// 行
	{
		for(int j=0;j<n;++j)
			tmp[j]=a[i][j];
		ans=max(ans,circ());
	}
	for(int j=0;j<n;++j)	// 列
	{
		for(int i=0;i<n;++i)
			tmp[i]=a[i][j];
		ans=max(ans,circ());
	}
	for(int s=0;s<n;++s)	// 主对角线方向：(s,0) 出发右下绕环
	{
		for(int k=0;k<n;++k)
			tmp[k]=a[(s+k)%n][k];
		ans=max(ans,circ());
	}
	for(int s=0;s<n;++s)	// 副对角线方向：(s,0) 出发右上绕环
	{
		for(int k=0;k<n;++k)
			tmp[k]=a[(s-k+n)%n][k];
		ans=max(ans,circ());
	}
	cout<<ans<<'\n';
	return 0;
}
