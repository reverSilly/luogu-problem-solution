//
// 题目：P1512 [ICPC 2001 Taejon R] 伊甸园日历游戏
// https://www.luogu.com.cn/problem/P1512
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxy{2010};          // 年份上界 2006 (+松弛)
constexpr int md[13]{0,31,28,31,30,31,30,31,31,30,31,30,31};

bool win[mxy][15][35];            // win[y][m][d]=从 y.m.d 出发先手是否必胜

bool isLeap(int y){return y%400==0||(y%4==0&&y%100!=0);}
int dim(int y,int m){return m==2?28+isLeap(y):md[m];}

// 判断日期是否在目标 2006.11.4 之前(含 2006.11.4)
bool validTarget(int y,int m,int d)
{
	if(d>dim(y,m))
		return false;
	return tie(y,m,d)<=make_tuple(2006,11,4);
}

void ini()
{
	// 终点 2006.11.4: 上一步走到的玩家已获胜, 当前轮到的玩家处于必败态(false)
	win[2006][11][4]=false;
	int y=2006,m=11,d=3;
	while(true)
	{
		bool w=false;
		// 操作 1: 天数 +1 (月底则进位到下月 1 日)
		int y1=y,m1=m,d1=d+1;
		if(d1>dim(y1,m1))
		{
			d1=1;
			if(++m1>12)
				m1=1,++y1;
		}
		if(validTarget(y1,m1,d1)&&!win[y1][m1][d1])
			w=true;

		// 操作 2: 月份 +1 (日号不变, 若目标月无该日或超界则非法)
		int y2=y,m2=m+1,d2=d;
		if(m2>12)
			m2=1,++y2;
		if(validTarget(y2,m2,d2)&&!win[y2][m2][d2])
			w=true;

		win[y][m][d]=w;
		if(y==1900&&m==1&&d==1)
			break;
		if(--d==0)
		{
			if(--m==0)
				m=12,--y;
			d=dim(y,m);
		}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	ini();
	int t;
	if(!(cin>>t))
		return 0;
	for(int i=0,y,m,d;i<t;++i)
	{
		cin>>y>>m>>d;
		cout<<(win[y][m][d]?"YES\n":"NO\n");
	}
	return 0;
}
