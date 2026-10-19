// 题目：P1548 [NOIP 1997 普及组] 棋盘问题
// https://www.luogu.com.cn/problem/P1548
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n,m;
	cin>>n>>m;
	i64 sq=0;                                     // 正方形个数
	for(int k=1;k<=min(n,m);++k)                  // 边长为 k 的正方形
		sq+=(i64)(n-k+1)*(m-k+1);
	i64 tot=(i64)n*(n+1)/2*(m*(m+1)/2);           // 全部矩形 = C(n+1,2)*C(m+1,2)
	cout<<sq<<' '<<tot-sq<<'\n';                  // 长方形 = 矩形 - 正方形
	return 0;
}
