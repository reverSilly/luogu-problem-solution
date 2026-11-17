// 题目：P2946 [USACO09MAR] Cow Frisbee Team S
// 子集和模 F 计数：f[j]=已处理的牛中选出子集、能力和 ≡j (mod F) 的方案数（模 1e8）
// 每头牛只有 选/不选 两种决策 ⇒ 0/1 背包；余数会回绕，不能原地倒序，必须复制一份
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005},p{100000000};

int n,F,f[mxn],g[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>F;
	f[0]=1;	// 空集：和为 0，余数 0，恰好贡献 1
	for(int i=0,r;i<n;++i)
	{
		cin>>r;r%=F;
		copy_n(f,F,g);	// g 先继承「不选第 i 头」
		for(int j=0;j<F;++j)
			g[(j+r)%F]=(g[(j+r)%F]+f[j])%p;	// 「选第 i 头」：余数整体平移 r
		copy_n(g,F,f);
	}
	cout<<(f[0]-1+p)%p<<'\n';	// 扣掉空集；f[0] 取模后可能为 0，先 +p 防负
	return 0;
}
