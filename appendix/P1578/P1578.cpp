//
// 题目：P1578 [WC2002] 奶牛浴场
// https://www.luogu.com.cn/problem/P1578
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{5010};          // n<=5000 (+4 角点 + 松弛)

struct Pt
{
	int x,y;
}p[mxn];

int L,W,n;

bool cmp_x(Pt a,Pt b){return a.x!=b.x?a.x<b.x:a.y<b.y;}
bool cmp_y(Pt a,Pt b){return a.y!=b.y?a.y<b.y:a.x<b.x;}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>L>>W>>n))
		return 0;
	for(int i=0;i<n;++i)
		cin>>p[i].x>>p[i].y;

	// 加入四个角点以便统一处理贯穿全宽或全高的条带
	p[n++]={0,0};
	p[n++]={0,W};
	p[n++]={L,0};
	p[n++]={L,W};

	long long ans=0;

	// 1. 高度占满 [0, W] 的竖直条带 + 左右由障碍点限制的极大空矩形
	sort(p,p+n,cmp_x);
	for(int i=0;i+1<n;++i)
		ans=max(ans,1LL*(p[i+1].x-p[i].x)*W);

	for(int i=0;i<n;++i)
	{
		int low=0,high=W;
		for(int j=i+1;j<n;++j)
		{
			if(p[j].x>p[i].x)
				ans=max(ans,1LL*(p[j].x-p[i].x)*(high-low));
			if(p[j].y>p[i].y&&p[j].y<high)
				high=p[j].y;
			else if(p[j].y<p[i].y&&p[j].y>low)
				low=p[j].y;
			else if(p[j].y==p[i].y&&p[j].x>p[i].x)
				break;
		}
		low=0;high=W;
		for(int j=i-1;j>=0;--j)
		{
			if(p[j].x<p[i].x)
				ans=max(ans,1LL*(p[i].x-p[j].x)*(high-low));
			if(p[j].y>p[i].y&&p[j].y<high)
				high=p[j].y;
			else if(p[j].y<p[i].y&&p[j].y>low)
				low=p[j].y;
			else if(p[j].y==p[i].y&&p[j].x<p[i].x)
				break;
		}
	}

	// 2. 宽度占满 [0, L] 的水平条带
	sort(p,p+n,cmp_y);
	for(int i=0;i+1<n;++i)
		ans=max(ans,1LL*(p[i+1].y-p[i].y)*L);

	cout<<ans<<'\n';
	return 0;
}
