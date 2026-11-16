// 题目：P2755 洗牌问题
// 洗牌是位置置换 f(i)≡2i (mod 2n+1)；整副恢复初始 ⟺ 2^m≡1 (mod 2n+1)
// 故 m = 2 模 (2n+1) 的乘法阶，反复翻倍取模即可
// x<M<=2e8+1 ⇒ 2x<2^31，用 int 条件减法代替慢速 %，极限 0.14s
#include<bits/stdc++.h>
using namespace std;

int n,ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	int M=2*n+1;		// 模数，奇数 ⇒ gcd(2,M)=1，阶存在
	int x=2;			// M>=3 ⇒ 2%M=2，即 2^1
	ans=1;
	while(x!=1)			// 第一次 2^ans≡1 即乘法阶
	{
		x+=x;			// 2x<4e8+2<int 上限，条件减法 = 取模
		if(x>=M)
			x-=M;
		++ans;
	}
	cout<<ans<<'\n';
	return 0;
}
