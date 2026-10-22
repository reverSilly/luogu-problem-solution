// 题目：P2181 对角线
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
using i128=__int128_t;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	i64 n;
	cin>>n;
	// 交点数等于 C(n,4)：任取 4 个顶点恰确定一个交点
	// n<=1e5 时 C(n,4) 约 4.17e18，超过 int64 中间乘积，故用 __int128
	i128 res=(i128)n*(n-1)*(n-2)*(n-3)/24;
	cout<<(i64)res<<'\n';
	return 0;
}
