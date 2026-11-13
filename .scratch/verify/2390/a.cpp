// 题目：P2390 地标访问
// 思路：连续行走 ⇒ 访问集合 = 排序后一段跨 0 的区间 [L,R]
//       覆盖 [L,R] 的最短路程 = min(R-2L,2R-L) = (R-L)+min(-L,R)
//       端点可收缩到 0 或地标坐标；枚举左侧访问 a 个负地标（最靠近 0 的 a 个）
//       d=-L 固定后，R 的可行上界 = max(t-2d, (t-d)/2)，upper_bound 数右侧个数
//       只枚举左侧即覆盖最优（两种行走顺序已在 max 里）
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{50005};

int n,x[mxn];
i64 t;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>t>>n;                       // 注意：t 在前，n 在后
	for(int i=0;i<n;++i)
		cin>>x[i];
	sort(x,x+n);

	int p=lower_bound(x,x+n,0)-x;    // x[0..p-1] 为负坐标
	int z=(p<n&&x[p]==0);            // 原点处的地标白送
	int ans=0;
	for(int a=0;a<=p;++a)            // 枚举访问最靠近原点的 a 个负坐标地标
	{
		i64 d=a?-x[p-a]:0;           // 向左走的最远距离
		if(d>t)
			break;                   // d 随 a 严格递增，后面全不可行
		// 先左后右：R+2d<=t；先右后左：2R+d<=t
		i64 R=max({t-2*d,(t-d)/2,0LL});
		int cnt=upper_bound(x+p+z,x+n,R)-(x+p+z);
		ans=max(ans,a+z+cnt);
	}
	cout<<ans<<'\n';
	return 0;
}
