// 题目：P2386 放苹果
// 思路：盘子相同 ⇒ 等价于「m 的至多 n 部整数拆分」
//       按「有没有空盘」分类：f(m,n)=f(m,n-1)+f(m-n,n)  (n<=m)
//       n>m 时多出的盘子必空：f(m,n)=f(m,m)
//       m,n<=10，一次性填表，t 组询问 O(1) 查表
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{15};

int t,m,n,f[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	// f[i][j]：i 个相同苹果放进 j 个相同盘子（允许空盘）的方案数
	// 第 0 行必须显式置 1：没有苹果只有「全空」这一种方案
	for(int j=0;j<mxn;++j)
		f[0][j]=1;
	for(int i=1;i<mxn;++i)
		for(int j=1;j<mxn;++j)
		{
			if(j>i)
				f[i][j]=f[i][i];      // 盘子比苹果多，多出来的必然空着
			else
				f[i][j]=f[i][j-1]+f[i-j][j];
		}

	cin>>t;
	while(t--)
	{
		cin>>m>>n;
		cout<<f[m][n]<<'\n';
	}
	return 0;
}
