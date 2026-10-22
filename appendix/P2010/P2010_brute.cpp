// 题目：P2010 [NOIP 2016 普及组] 回文日期
// 暴力：逐日枚举区间内所有真实日期，检查 8 位表示是否为回文
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{5};

int d1,d2;

bool isleap(int y)
{
	return (y%4==0&&y%100!=0)||(y%400==0);
}

int mdays(int y,int m)
{
	static const int md[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
	if(m==2&&isleap(y))
		return 29;
	return md[m];
}

bool ispal(int x)
{
	int a[8],t=x;
	for(int i=0;i<8;++i)
	{
		a[i]=t%10;
		t/=10;
	}
	for(int i=0;i<4;++i)
		if(a[i]!=a[7-i])
			return false;
	return true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>d1>>d2;
	int y=d1/10000,m=d1/100%10,da=d1%100;
	int ans=0;
	while(true)
	{
		int cur=y*10000+m*100+da;
		if(cur>d2)
			break;
		if(ispal(cur))
			++ans;
		++da;
		if(da>mdays(y,m))
		{
			da=1;
			++m;
			if(m>12)
			{
				m=1;
				++y;
			}
		}
	}
	cout<<ans<<'\n';
	return 0;
}
