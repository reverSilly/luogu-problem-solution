#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int T,N,Y,M,D,h,m,s;

bool leap(int y){return (y%4==0&&y%100!=0)||y%400==0;}
int mdays(int y,int mo)
{
	static int d[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
	if(mo==2&&leap(y))
		return 29;
	return d[mo];
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>T;
	while(T--)
	{
		cin>>N>>Y>>M>>D>>h>>m>>s;
		i64 S=(1LL<<(N-1))-1;		// N 位有符号最大值 = 最后有效时刻的秒数
		i64 tot=h*3600LL+m*60+s+S;
		int days=tot/86400;
		i64 rem=tot%86400;
		h=rem/3600;
		m=rem%3600/60;
		s=rem%60;
		while(days>0)				// 把 days 天加到日期上，逐月跳
		{
			int dim=mdays(Y,M);
			if(D+days<=dim)
			{
				D+=days;
				days=0;
			}
			else
			{
				days-=dim-D+1;		// 跳到下月 1 号
				D=1;
				++M;
				if(M==13)
				{
					M=1;
					++Y;
				}
			}
		}
		cout<<Y<<' '<<M<<' '<<D<<' '<<h<<' '<<m<<' '<<s<<'\n';
	}
	return 0;
}