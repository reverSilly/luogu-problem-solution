// 题目：P2242 公路维修问题
// 思路：m 段 = 在相邻坑之间切 m-1 刀；一段 [l,r] 的长度按题面样例是 r-l+1，
//       故基准为 a[n-1]-a[0]+1，每切一刀减少 g-1，切最大的 m-1 个间隔
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{15005};

int n,m;
i64 a[mxn],g[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int i=1;i<n;++i)
		g[i]=a[i]-a[i-1];
	sort(g+1,g+n,greater<>());   // g[1..n-1] 降序

	i64 ans=a[n-1]-a[0]+1;       // 一整段的长度（闭区间整数点个数）
	for(int i=1;i<=m-1;++i)
		ans-=g[i]-1;             // 切开的间隔不再被覆盖，省下 g-1
	cout<<ans<<'\n';
	return 0;
}
