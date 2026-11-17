// 题目：P2983 [USACO10FEB] Chocolate Buying S
// 按单价升序贪心买满；先除后乘防溢出（P*C 可达 1e36）
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

int n;
i64 b,ans;
struct node{i64 p,c;}a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>b;
	for(int i=0;i<n;++i)
		cin>>a[i].p>>a[i].c;
	sort(a,a+n,[](node x,node y){return x.p<y.p;});
	for(int i=0;i<n&&b>0;++i)
	{
		i64 t=min(a[i].c,b/a[i].p);	// 先除后乘：t*p<=b<=1e18，绝不直接算 p*c
		ans+=t;
		b-=t*a[i].p;
	}
	cout<<ans<<'\n';
	return 0;
}