// 题目：P2807 三角形计数
// 朝上边长 k：T(n-k+1)；朝下边长 k：T(n-2k+1)(需 2k<=n)
// 两类求和化简 = floor(n(n+2)(2n+1)/8)，正整数除法即下取整
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int t;
	cin>>t;
	while(t--)
	{
		i64 n;
		cin>>n;
		cout<<n*(n+2)*(2*n+1)/8<<'\n';	// n 奇时乘积 ≡1(mod 8)，截断恰好减 1/8
	}
	return 0;
}
