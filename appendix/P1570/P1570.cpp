//
// 题目：P1570 KC 喝咖啡
// https://www.luogu.com.cn/problem/P1570
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{205};       // n<=200 (+5 松弛)
constexpr int mxr{1000};      // 答案上界(题面保证不超过 1000)

int n,m;
double v[mxn],c[mxn],d[mxn];  // d[i]=v_i - lambda*c_i

bool check(double lam)
{
	for(int i=0;i<n;++i)
		d[i]=v[i]-lam*c[i];
	sort(d,d+n,greater<>());  // 选恰好 m 项,贪心取最大的 m 个
	double s=0;
	for(int i=0;i<m;++i)
		s+=d[i];
	return s>=0;              // 存在 m 项之和 >=0 <=> 比值可达 >= lam
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>v[i];
	for(int i=0;i<n;++i)
		cin>>c[i];

	double lo=0,hi=mxr;       // lo 恒可行(v_i>=1),hi=答案上界
	for(int it=0;it<100;++it)
	{
		double mid=(lo+hi)/2;
		if(check(mid))
			lo=mid;
		else
			hi=mid;
	}
	cout<<fixed<<setprecision(3)<<lo<<'\n';
	return 0;
}
