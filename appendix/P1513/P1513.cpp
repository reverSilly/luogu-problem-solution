//
// 题目：P1513 绕钉子的长绳子
// https://www.luogu.com.cn/problem/P1513
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};   // N<=100 (+5 松弛)

int n;
double R;
double x[mxn],y[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>R;
	for(int i=0;i<n;++i)
		cin>>x[i]>>y[i];

	double per=0;                    // 相邻钉子中心连线的总长 = 直线段总长
	for(int i=0;i<n;++i)
	{
		int j=(i+1)%n;
		per+=hypot(x[i]-x[j],y[i]-y[j]);
	}
	double ans=per+2*acos(-1.0)*R;   // 凸多边形外角和为 2π => 圆弧总长 2πR
	cout<<fixed<<setprecision(2)<<ans<<'\n';
	return 0;
}
