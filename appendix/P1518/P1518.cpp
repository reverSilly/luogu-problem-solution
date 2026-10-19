// 题目：P1518 [USACO2.4] 两只塔姆沃斯牛 The Tamworth Two
// https://www.luogu.com.cn/problem/P1518
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{12};
constexpr int mxst{160005};               // (100 格 * 4 向)^2 = 160000 (+5 松弛)

const int dr[4]={-1,0,1,0},dc[4]={0,1,0,-1};   // 0北 1东 2南 3西

char g[mxn][mxn];
bool vis[mxst];                           // 全局默认 false

void step(int&r,int&c,int&d)              // 前进一分钟(原地改写 r,c,d)
{
	int nr=r+dr[d],nc=c+dc[d];
	if(nr<0||nr>=10||nc<0||nc>=10||g[nr][nc]=='*')
		d=(d+1)%4;                        // 前方受阻(边界也算障碍):原地右转
	else
		r=nr,c=nc;                        // 否则前进一步
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

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
	int jd=0,cd=0;                        // 两者最初都朝北
	int t=0;
	for(;;)
	{
		int st=(jr*10+jc)*4+jd;           // John 位置(100)+方向(4)
		st=st*400+(cr*10+cc)*4+cd;        // 拼上牛的状态 => 160000 种
		if(vis[st])                       // 状态重现 => 进入循环,永不相遇
		{
			cout<<0<<'\n';
			return 0;
		}
		vis[st]=true;
		step(jr,jc,jd);
		step(cr,cc,cd);
		++t;
		if(jr==cr&&jc==cc)                // 分钟末同格 => 抓住
		{
			cout<<t<<'\n';
			return 0;
		}
	}
}
