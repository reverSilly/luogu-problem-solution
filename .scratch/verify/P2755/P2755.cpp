#include<bits/stdc++.h>
using namespace std;

int n,ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	int M=2*n+1;		// 洗牌置换 ≡ 乘 2 (mod 2n+1)，M 为奇数
	int x=2;			// M>=3 ⇒ 2%M=2，即 2^1
	ans=1;
	while(x!=1)			// 求 2 模 M 的乘法阶
	{
		x+=x;			// x<M<=2e8+1 ⇒ 2x<4e8+2<int 上限，条件减法代替取模
		if(x>=M)
			x-=M;
		++ans;
	}
	cout<<ans<<'\n';
	return 0;
}