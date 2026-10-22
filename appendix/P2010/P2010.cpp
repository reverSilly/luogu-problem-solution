// 题目：P2010 [NOIP 2016 普及组] 回文日期
// 枚举年份：由年份唯一确定候选回文日期 abcd -> dc 月 ba 日
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

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

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>d1>>d2;
	int y1=d1/10000,y2=d2/10000;
	int ans=0;
	for(int y=y1;y<=y2;++y)
	{
		//回文日期 abcd dc ba
		int mo=(y%10)*10+(y/10)%10;
		int da=(y/100%10)*10+y/1000;
		if(mo<1||mo>12)
			continue;
		if(da<1||da>mdays(y,mo))
			continue;
		int cur=y*10000+mo*100+da;
		if(cur>=d1&&cur<=d2)
			++ans;
	}
	cout<<ans<<'\n';
	return 0;
}
