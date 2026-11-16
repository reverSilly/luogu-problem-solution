// 题目：P2696 慈善的约瑟夫
// 每轮付款 n_i-n_{i+1}，望远镜求和 ⇒ 总付款 = N + 终态人数 F
// J(n)=2l+1（n=2^m+l）是二进制循环左移：保持 popcount，不动点为全 1 数
// ⇒ F=2^popcount(N)-1，答案一行：N+(1<<popcount(N))-1
#include<bits/stdc++.h>
using namespace std;

int n;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	int pc=__builtin_popcount(n);	// 循环移位不变量：1 的个数
	cout<<n+(1<<pc)-1<<'\n';	// N<=1e5 ⇒ pc<=16，答案<=165535，int 安全
	return 0;
}
