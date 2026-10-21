//
// 题目：P1847 轰炸II
// https://www.luogu.com.cn/problem/P1847
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2005};

struct rec
{
	i64 x1,y1,x2,y2;
}r[mxn];

int m,n;

bool hit(int i,i64 x,i64 y)
{
	return r[i].x1<=x&&x<=r[i].x2&&r[i].y1<=y&&y<=r[i].y2;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>m>>n;
	for(int i=0;i<m;++i)
	{
		cin>>r[i].x1>>r[i].y1>>r[i].x2>>r[i].y2;
		if(r[i].x1>r[i].x2)
			swap(r[i].x1,r[i].x2);
		if(r[i].y1>r[i].y2)
			swap(r[i].y1,r[i].y2);
	}
	for(int i=0;i<n;++i)
	{
		i64 x,y;
		cin>>x>>y;
		int cnt=0,last=0;
		for(int j=0;j<m;++j)
		{
			if(hit(j,x,y))
			{
				++cnt;
				last=j+1;
			}
		}
		if(cnt==0)
			cout<<"NO\n";
		else
			cout<<"YES "<<cnt<<' '<<last<<'\n';
	}
	return 0;
}
