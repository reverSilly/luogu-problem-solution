// 题目：P2241 统计方形（数据加强版）
// 思路：矩形总数 = C(n+1,2)*C(m+1,2)；正方形按边长 k 统计，每类 (n-k+1)(m-k+1) 个
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 n,m;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	i64 sq=0;
	for(i64 k=1;k<=min(n,m);++k)
		sq+=(n-k+1)*(m-k+1);
	i64 tot=(n*(n+1)/2)*(m*(m+1)/2);
	cout<<sq<<' '<<tot-sq<<'\n';
	return 0;
}
