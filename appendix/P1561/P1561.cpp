//
// 题目：P1561 [USACO12JAN] Mountain Climbing S
// https://www.luogu.com.cn/problem/P1561
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{25005};         // n<=25000 (+5 松弛)

int n;
struct{int u,d;}a[mxn];           // u=上山时间, d=下山时间

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i].u>>a[i].d;
	// Johnson 排序: 上山时间<=下山时间的一组在前(按上山升序),
	// 其余在后(按下山降序)
	sort(a,a+n,[](auto x,auto y){
		bool gx=x.u<=x.d,gy=y.u<=y.d;
		if(gx!=gy)
			return gx>gy;
		if(gx)
			return x.u<y.u;
		return x.d>y.d;
	});
	i64 remD{0},curU{0},ans{0};
	for(int i=0;i<n;++i)
		remD+=a[i].d;             // 先累计出总下山时间
	for(int i=0;i<n;++i)
	{
		curU+=a[i].u;
		ans=max(ans,curU+remD);   // f(k)=上山前缀+下山后缀
		remD-=a[i].d;
	}
	cout<<ans<<'\n';
	return 0;
}
