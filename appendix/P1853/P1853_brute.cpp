//
// 题目：P1853 [NWERC 2004] 投资的最大效益 暴力（按件数深搜）
// https://www.luogu.com.cn/problem/P1853
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxd{12};

int d,a[mxd],b[mxd];
int best;

void dfs(int i,int left,int gain)
{
	if(i==d)
	{
		if(gain>best)
			best=gain;
		return;
	}
	for(int c=0;;++c)
	{
		if((i64)c*a[i]>left)
			break;
		dfs(i+1,left-(int)((i64)c*a[i]),gain+c*b[i]);
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int money,yr;
	cin>>money>>yr>>d;
	for(int i=0;i<d;++i)
		cin>>a[i]>>b[i];
	for(int y=0;y<yr;++y)
	{
		best=0;
		dfs(0,money,0);
		money+=best;
	}
	cout<<money<<'\n';
	return 0;
}
