//
// 题目：P1586 四方定理
// https://www.luogu.com.cn/problem/P1586
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{32773};   // n<=32768 (+5 松弛)
constexpr int mxk{182};     // 底数上界:181^2=32761<=32768<182^2
constexpr int mxpart{5};    // 部件数 0..4

int f[mxn][mxpart];         // f[j][c]=面额类枚举到当前,和为 j 且恰好 c 枚的多重集合数

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	f[0][0]=1;              // 空多重集合是 DP 唯一种子
	for(int k=1;k<mxk;++k)  // 面额 k^2 逐类加入;j 正序 -> 同面额可取多次(完全背包,非 0-1)
		for(int j=k*k;j<mxn;++j)
			for(int c=1;c<mxpart;++c)
				f[j][c]+=f[j-k*k][c-1];
	int t;
	cin>>t;
	while(t--)
	{
		int n;
		cin>>n;
		cout<<f[n][1]+f[n][2]+f[n][3]+f[n][4]<<'\n';   // 1~4 枚即"不超过四个"
	}
	return 0;
}

// 样例输入：
// 1
// 2003
