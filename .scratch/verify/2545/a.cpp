// 题目：P2545 [AHOI2004] 实验基地
// 思路：凹字型 = 选 l<a<=b<r，收益 (B_r-B_{l-1})+(A_{a-1}-A_{l-1})+(A_r-A_b)。
//       换元 x=l-1,y=a-1,z=b,w=r ⇒ 约束 x<y<z<w，收益 C_w-C_x+A_y-A_z（C=A+B）。
//       四阶段 DP：dp1=-C_x, dp2=A_y-C_x, dp3=A_y-C_x-A_z，扫到 i 先当 w 更新 ans，
//       再依次接进 dp3/dp2/dp1；每阶段读上一阶段旧值保证严格递增。O(N)。
//       值域 ±8e9 超 int ⇒ long long；哨兵 -2^60 只参与 max。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2005};
constexpr i64 ninf{-(1LL<<60)};

int n;
i64 a[mxn],c[mxn];	// a=第一行前缀和, c=两行合计前缀和

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1;i<=n;++i)
	{
		i64 x;cin>>x;
		a[i]=a[i-1]+x;
	}
	for(int i=1;i<=n;++i)
	{
		i64 x;cin>>x;
		c[i]=c[i-1]+(a[i]-a[i-1])+x;	// a[i]-a[i-1] 即第一行第 i 格
	}
	// 凹字型 = 选 x<y<z<w，收益 c[w]-c[x]+a[y]-a[z]
	i64 dp1=ninf,dp2=ninf,dp3=ninf,ans=ninf;
	for(int i=0;i<=n;++i)
	{
		if(dp3>ninf/2)
			ans=max(ans,dp3+c[i]);	// i 当 w
		if(dp2>ninf/2)
			dp3=max(dp3,dp2-a[i]);	// i 当 z（读旧 dp2，y<i）
		if(dp1>ninf/2)
			dp2=max(dp2,dp1+a[i]);	// i 当 y（读旧 dp1，x<i）
		dp1=max(dp1,-c[i]);	// i 当 x
	}
	cout<<ans<<'\n';
	return 0;
}
