// 题目：P2642 最大双子段和
// 思路：两段至少间隔一个数 ⇒ 存在切割 g，左段在 [0,g-1]、右段在 [g+1,n-1]，两区域独立。
//       lf[i]=前缀最大非空子段和，rt[i]=后缀最大非空子段和，各一遍 Kadane。
//       ans = max_{g=1..n-2} (lf[g-1]+rt[g+1])。
//       非空强制：cur=max(a[i],cur+a[i])，全负时答案为负，绝不能引入空段得 0。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1000005};
constexpr i64 inf{1145141919810};

int n;
i64 a[mxn],lf[mxn],rt[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];
	// lf[i]：a[0..i] 内最大非空子段和；rt[i]：a[i..n-1] 内最大非空子段和
	i64 cur=a[0];lf[0]=a[0];
	for(int i=1;i<n;++i)
	{
		cur=max(a[i],cur+a[i]);	// 以 i 结尾：单点 a[i] 或接上一段
		lf[i]=max(lf[i-1],cur);
	}
	cur=a[n-1];rt[n-1]=a[n-1];
	for(int i=n-2;i>=0;--i)
	{
		cur=max(a[i],cur+a[i]);	// 以 i 开头：单点 a[i] 或接下一段
		rt[i]=max(rt[i+1],cur);
	}
	i64 ans=-inf;
	for(int g=1;g+1<n;++g)	// g 是间隔的那个数：左段 [0,g-1]，右段 [g+1,n-1]
		ans=max(ans,lf[g-1]+rt[g+1]);
	cout<<ans<<'\n';
	return 0;
}
