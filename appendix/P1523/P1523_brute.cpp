//
// 题目：P1523 旅行商简化版
// 暴力基准: 排序后枚举中间每个点归属上链(0)还是下链(1)的 2^(n-2) 种分配方案
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{25};
constexpr double inf{1e30};

int n;
struct Pt
{
	i64 x,y;
	friend bool operator<(Pt a,Pt b){return a.x!=b.x?a.x<b.x:a.y<b.y;}
}p[mxn];

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
	sort(p,p+n);

	int m=n-2;
	double ans=inf;
	for(int mask=0;mask<(1<<m);++mask)
	{
		int last0=0,last1=0;
		double cur=0;
		for(int i=1;i<=m;++i)
		{
			if(mask>>(i-1)&1)
				cur+=euc(last1,i),last1=i;
			else
				cur+=euc(last0,i),last0=i;
		}
		cur+=euc(last0,n-1)+euc(last1,n-1);
		ans=min(ans,cur);
	}
	cout<<fixed<<setprecision(2)<<ans<<'\n';
	return 0;
}
