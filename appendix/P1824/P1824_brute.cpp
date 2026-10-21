//
// 题目：P1824 [USACO05FEB] 进击的奶牛 Aggressive Cows G
// https://www.luogu.com.cn/problem/P1824
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25};
constexpr int inf{1145141919};

int n,m,best;
int a[mxn];

void dfs(int i,int taken,int last,int mind)
{
	if(taken==m)
	{
		best=max(best,mind);
		return;
	}
	for(int j=i;j<n;++j)
	{
		if(n-j<m-taken)
			break;
		if(taken==0)
			dfs(j+1,1,j,inf);
		else
			dfs(j+1,taken+1,j,min(mind,a[j]-a[last]));
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>a[i];
	sort(a,a+n);
	best=0;
	dfs(0,0,0,inf);
	cout<<best<<'\n';
	return 0;
}
