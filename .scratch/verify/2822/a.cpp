// 题目：P2822 [NOIP 2016 提高组] 组合数问题
// k 多为合数 ⇒ 阶乘+逆元不可行；杨辉三角只用加法，模任意 k 封闭
// 整份数据 k 固定 ⇒ 一次预处理"是否被 k 整除"的二维前缀和，询问 O(1)
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{2005},mxv{2000};

int t,k,c[mxn],s[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>t>>k;
	c[0]=1%k;					// k=1 时为 0，全部数对都计数，无需特判
	for(int i=0;i<=mxv;++i)
	{
		for(int j=i;j>=1;--j)
			c[j]=(c[j]+c[j-1])%k;		// 必须降序：c[j-1] 要保留上一行的值
		for(int j=0;j<=mxv;++j)
			s[i+1][j+1]=s[i][j+1]+s[i+1][j]-s[i][j]+(j<=i&&c[j]==0);
			// j<=i 不能省！j>i 处 c=0 是"幽灵格子"，不是合法数对
	}
	for(int i=0,n,m;i<t;++i)
	{
		cin>>n>>m;
		cout<<s[n+1][min(n,m)+1]<<'\n';	// 前缀和下标平移 +1，零边界天然正确
	}
	return 0;
}
