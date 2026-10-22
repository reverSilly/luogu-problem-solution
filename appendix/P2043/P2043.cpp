// 题目：P2043 质因子分解
// 思路：先筛出不超过 N 的所有质数，再用 Legendre 公式求每个质数在 N! 中的指数
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005};

bool vis[mxn];
int pri[mxn],cnt;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	// 埃氏筛：标出合数，同时收集质数
	for(int i=2;i<=n;++i)
	{
		if(vis[i]) continue;
		pri[++cnt]=i;
		for(int j=i*i;j<=n;j+=i) vis[j]=true;
	}
	for(int i=1;i<=cnt;++i)
	{
		int p=pri[i],a=0,x=n;
		// Legendre: v_p(N!)=N/p+N/p^2+...
		while(x)
		{
			x/=p;
			a+=x;
		}
		cout<<p<<' '<<a<<'\n';
	}
	return 0;
}
