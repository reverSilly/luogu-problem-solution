// 题目：P3062 [USACO12DEC] Wifi Setup S
// 最优解 = 连续分段，每段一个中点基站，花费 A+B*span/2。
// 全程乘 2 化整数：F[i]=min_j(F[j]+2A+B*(x[i-1]-x[j]))。
// 转移可分离 ⇒ 前缀 min 维护 F[j]-B*x[j]，O(N) 完成 DP。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2005};

int n,A,B,x[mxn];
i64 f[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>A>>B;
	for(int i=0;i<n;++i)
		cin>>x[i];
	sort(x,x+n);
	i64 mn=-(i64)B*x[0];	// min_{j<i}(f[j]-B*x[j])，初始只有 j=0（f[0]=0）
	for(int i=1;i<=n;++i)
	{
		f[i]=mn+2*A+(i64)B*x[i-1];	// 最后一段 x[j..i-1]，j 由 mn 隐含取最优
		if(i<n)
			mn=min(mn,f[i]-(i64)B*x[i]);
	}
	if(f[n]%2==0)
		cout<<f[n]/2<<'\n';
	else
		cout<<f[n]/2<<".5\n";	// 答案必为 0.5 的整数倍
	return 0;
}
