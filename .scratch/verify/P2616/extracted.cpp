// 题目：P2616 [USACO10JAN] Buying Feed, II S
// 思路：运费对载重线性 ⇒ 每吨饲料独立计费：在 x 处买一吨花费 C+(E-x)。
//       问题塌缩成「N 种物品、每种 F_i 件、单价 p_i=C_i+E-X_i，买恰好 K 件」
//       ⇒ 按有效单价排序贪心装容量，交换论证保证最优。O(N log N)。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{105};

int k,e,n;
struct node{int x,f,c;}a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>k>>e>>n;
	for(int i=0;i<n;++i)
		cin>>a[i].x>>a[i].f>>a[i].c;
	// E 是常数，按 C-X 排序等价于按有效单价 C+(E-X) 排序
	sort(a,a+n,[](node u,node v){return u.c-u.x<v.c-v.x;});
	i64 ans=0;
	for(int i=0;i<n&&k;++i)
	{
		int t=min(k,a[i].f);
		ans+=(i64)t*(a[i].c+e-a[i].x);
		k-=t;
	}
	cout<<ans<<'\n';
	return 0;
}
