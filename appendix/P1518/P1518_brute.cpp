// P1518 暴力基准：不做状态判重, 直接模拟足够多的分钟数
#include<bits/stdc++.h>
using namespace std;

const int dr[4]={-1,0,1,0},dc[4]={0,1,0,-1};
char g[12][12];

void step(int&r,int&c,int&d)
{
	int nr=r+dr[d],nc=c+dc[d];
	if(nr<0||nr>=10||nc<0||nc>=10||g[nr][nc]=='*')
		d=(d+1)%4;
	else
		r=nr,c=nc;
}

int main()
{
	int jr=0,jc=0,cr=0,cc=0;
	for(int i=0;i<10;++i)
		for(int j=0;j<10;++j)
		{
			cin>>g[i][j];
			if(g[i][j]=='F')
				jr=i,jc=j;
			if(g[i][j]=='C')
				cr=i,cc=j;
		}
	int jd=0,cd=0,t=0;
	while(t<200000)                       // 状态数上限 160000, 足够
	{
		step(jr,jc,jd);
		step(cr,cc,cd);
		++t;
		if(jr==cr&&jc==cc)
		{
			cout<<t<<'\n';
			return 0;
		}
	}
	cout<<0<<'\n';
	return 0;
}
