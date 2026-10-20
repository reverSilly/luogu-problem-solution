//
// 题目：P1562 还是 N 皇后
// https://www.luogu.com.cn/problem/P1562
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{20};            // n<=14 (+5 松弛)

int n;
int rowok[mxn];                   // rowok[r]=第 r 行可放棋子的列的位掩码
int ans{0};

void dfs(int r,int col,int d1,int d2)
{
	if(r==n)
	{
		++ans;
		return;
	}
	int avail=rowok[r]&~(col|d1|d2);   // 本行仍可放的低位集合
	while(avail)
	{
		int b=avail&-avail;       // 取最低位
		avail-=b;
		dfs(r+1,col|b,(d1|b)<<1,(d2|b)>>1);
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		string s;
		cin>>s;
		rowok[i]=0;
		for(int j=0;j<n;++j)
			if(s[j]=='*')
				rowok[i]|=1<<j;
	}
	dfs(0,0,0,0);
	cout<<ans<<'\n';
	return 0;
}
