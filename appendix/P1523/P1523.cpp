//
// 题目：P1523 旅行商简化版
// https://www.luogu.com.cn/problem/P1523
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005};          // n<=1000 (+5 松弛)
constexpr double inf{1e30};

int n;
struct Pt
{
	i64 x,y;
	friend bool operator<(Pt a,Pt b){return a.x!=b.x?a.x<b.x:a.y<b.y;}
}p[mxn];

double dp[mxn];                   // dp[j] 表示当前推进到 i 时状态 (i, j) 的最小双链总长 (0<=j<i)

double euc(int i,int j)
{
	double dx=(double)p[i].x-(double)p[j].x;
	double dy=(double)p[i].y-(double)p[j].y;
	return hypot(dx,dy);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n))
		return 0;
	for(int i=0;i<n;++i)
		cin>>p[i].x>>p[i].y;

	if(n<=1)
	{
		cout<<"0.00\n";
		return 0;
	}
	sort(p,p+n);                  // 输入未保证按 x 升序! 必须先排序

	dp[0]=euc(0,1);               // i=1, j=0: 一条链到 1, 另一条链留在 0
	for(int i=2;i<n;++i)
	{
		double d_step=euc(i-1,i);
		double best=inf;
		for(int j=0;j<i-1;++j)
		{
			best=min(best,dp[j]+euc(j,i)); // j=i-1 的新状态: 点 i 接在原本处于 j 的那条链上
			dp[j]+=d_step;                 // j<i-1 的新状态: 点 i 接在原本处于 i-1 的那条链上
		}
		dp[i-1]=best;
	}

	double ans=inf;
	for(int j=0;j<n-1;++j)
		ans=min(ans,dp[j]+euc(j,n-1));
	cout<<fixed<<setprecision(2)<<ans<<'\n';
	return 0;
}
