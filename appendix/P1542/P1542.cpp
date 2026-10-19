//
// 题目：P1542 包裹快递
// https://www.luogu.com.cn/problem/P1542
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{200005};        // n<=2e5 (+5 松弛)

int n;
int x[mxn],y[mxn],s[mxn];         // 签收区间 [x_i,y_i] 与第 i 段距离 s_i

bool ok(double v)                 // 以最大速度 v 能否全部准时签收
{
	double t=0;                   // 当前(刚离开上一地点)的时刻
	for(int i=0;i<n;++i)
	{
		t+=s[i]/v;                // 全程用最大速度 => 到达第 i 个地点最早时刻
		if(t>y[i])                // 晚于最晚签收时刻: 不可行 (严格比较, 见验证小结)
			return false;
		if(t<x[i])                // 早到: 原地停留至可以签收
			t=x[i];
	}
	return true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>x[i]>>y[i]>>s[i];

	double lo=0,hi=2e7;           // 答案上界 1e7 (见题解引理), 取 2e7 宽松包裹
	for(int it=0;it<100;++it)     // 100 轮二分, 精度远优于两位小数
	{
		double mid=(lo+hi)/2;
		if(ok(mid))
			hi=mid;
		else
			lo=mid;
	}
	cout<<fixed<<setprecision(2)<<hi+1e-9<<'\n';   // 1e-9 远小于 0.005, 仅用于稳定半分位舍入
	return 0;
}
