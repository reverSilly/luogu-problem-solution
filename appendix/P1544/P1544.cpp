//
// 题目：P1544 三倍经验
// https://www.luogu.com.cn/problem/P1544
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{105};                    // n<=100 (+5 松弛)
constexpr i64 nf{-1'000'000'000'000'000'000};   // 负无穷哨兵 (|真实值|<=3e11)

int n,k;
i64 a[mxn][mxn];                           // a[i][j]=第 i 行(0-based)第 j 个数字
i64 dp[mxn][mxn];                          // dp[j][c]=本行停在位置 j 且已用 c 次三倍的最大得分
i64 prv[mxn][mxn];                         // prv=上一行的 dp

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	for(int i=0;i<n;++i)
		for(int j=0;j<=i;++j)
			cin>>a[i][j];

	int C=min(k,n);                        // 路径恰好经过 n 个格子, 三倍次数超过 n 无意义
	fill_n(&dp[0][0],mxn*mxn,nf);          // 全局默认 0 不是负无穷, 必须显式填充
	fill_n(&prv[0][0],mxn*mxn,nf);

	prv[0][0]=a[0][0];                     // 第 0 行只有一条来路: 不乘
	if(C>=1)
		prv[0][1]=3*a[0][0];               // 或把顶部数字乘 3

	for(int i=1;i<n;++i)
	{
		for(int j=0;j<=i;++j)
			for(int c=0;c<=C;++c)
			{
				i64 bnot=nf,bmul=nf;       // 本格不乘 / 本格乘 3 时的最优前驱
				if(j-1>=0)                 // 来自左上 (i-1,j-1)
				{
					bnot=max(bnot,prv[j-1][c]);
					if(c>=1)
						bmul=max(bmul,prv[j-1][c-1]);
				}
				if(j<=i-1)                 // 来自右上 (i-1,j)
				{
					bnot=max(bnot,prv[j][c]);
					if(c>=1)
						bmul=max(bmul,prv[j][c-1]);
				}
				i64 res=nf;
				if(bnot>nf/2)              // 本格不乘 3
					res=max(res,bnot+a[i][j]);
				if(bmul>nf/2)              // 本格乘 3, 消耗一次机会
					res=max(res,bmul+3*a[i][j]);
				dp[j][c]=res;
			}
		for(int j=0;j<=i;++j)
			for(int c=0;c<=C;++c)
				prv[j][c]=dp[j][c];
	}

	i64 ans=nf;                             // 留在某一列、用不超过 C 次三种情况取最大
	for(int j=0;j<n;++j)
		for(int c=0;c<=C;++c)
			ans=max(ans,prv[j][c]);
	cout<<ans<<'\n';
	return 0;
}
