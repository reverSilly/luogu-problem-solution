// 题目：P2878 [USACO07JAN] Protecting the Flowers S
// 邻项交换 ⇒ 按 T/D 升序（交叉相乘比较）；ans=Σ 2*T_i*(尚未赶回的 D 之和)
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

int n;
struct cow{int t,d;}a[mxn];
i64 ans,sumd;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		cin>>a[i].t>>a[i].d;
		sumd+=a[i].d;
	}
	sort(a,a+n,[](cow x,cow y){return (i64)x.t*y.d<(i64)y.t*x.d;});
	for(int i=0;i<n;++i)
	{
		sumd-=a[i].d;	// 正在被赶的牛不吃花
		ans+=2*a[i].t*sumd;
	}
	cout<<ans<<'\n';
	return 0;
}
