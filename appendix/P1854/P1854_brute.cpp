//
// 题目：P1854 [IOI 1999] 花店橱窗布置
// https://www.luogu.com.cn/problem/P1854
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};
constexpr int inf{1145141919};

int f,v;
int a[mxn][mxn];
int best;

// 枚举严格递增的花瓶下标，与标程的区间 DP 不同
void dfs(int i,int last,int sum)
{
	if(sum+50*(f-i+1)<=best)
		return;
	if(i==f+1)
	{
		best=max(best,sum);
		return;
	}
	for(int j=last+1;j<=v-(f-i);++j)
		dfs(i+1,j,sum+a[i][j]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>f>>v;
	for(int i=1;i<=f;++i)
		for(int j=1;j<=v;++j)
			cin>>a[i][j];
	best=-inf;
	dfs(1,0,0);
	cout<<best<<'\n';
	return 0;
}
