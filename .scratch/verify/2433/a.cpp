// 题目：P2433 【深基1-2】小学数学 N 合一
// 读题号 T，分发到 14 个小题；浮点一律裸 cout（默认 6 位有效数字）
#include<bits/stdc++.h>
using namespace std;

constexpr double pi{3.141593};	// 题面指定，不能用 acos(-1)

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int T;
	cin>>T;
	if(T==1)
		cout<<"I love Luogu!\n";
	else if(T==2)
		cout<<2+4<<' '<<10-2-4<<'\n';
	else if(T==3)
		cout<<14/4<<'\n'<<14/4*4<<'\n'<<14%4<<'\n';
	else if(T==4)
		cout<<500.0/3<<'\n';			// 6 位有效数字 166.667
	else if(T==5)
		cout<<(260+220)/(12+20)<<'\n';		// 相对运动 480/32=15
	else if(T==6)
		cout<<sqrt(9.0*9+6.0*6)<<'\n';		// 勾股 sqrt(117)
	else if(T==7)
		cout<<100+10<<'\n'<<100+10-20<<'\n'<<0<<'\n';
	else if(T==8)
		cout<<2*pi*5<<'\n'<<pi*5*5<<'\n'<<4.0/3*pi*5*5*5<<'\n';
	else if(T==9)
	{
		int x=1;				// 第 4 天剩 1，逆推 3 天
		for(int i=0;i<3;++i)
			x=(x+1)*2;
		cout<<x<<'\n';
	}
	else if(T==10)
	{
		// 牛吃草：8*30=S+30v, 10*6=S+6v
		double v=(8.0*30-10.0*6)/(30-6),s=10.0*6-6*v;
		cout<<(int)round((s+10*v)/10)<<'\n';
	}
	else if(T==11)
		cout<<100.0/(8-5)<<'\n';		// 追及 100/3
	else if(T==12)
		cout<<'M'-'A'+1<<'\n'<<(char)('A'+18-1)<<'\n';
	else if(T==13)
		cout<<(int)cbrt(4.0/3*pi*(4*4*4+10*10*10))<<'\n';	// 体积守恒，舍去小数
	else if(T==14)
		cout<<(int)round((120-sqrt(120.0*120-4*3500))/2)<<'\n';	// 取较小根
	return 0;
}
