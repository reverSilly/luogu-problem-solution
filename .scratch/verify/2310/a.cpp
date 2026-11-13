// 题目：P2310 loidc，看看海
// 为什么换轴：区间第 k 小是「在高度全序上数数」，所以先把高度序排出来（ord）
// 询问 [x,y] 的答案 = ord 上第 k 个落在 [x,y] 里的时刻（取子序列仍升序）
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{4005};

int n,m,a[mxn],ord[mxn];	//ord[v]=高度第 v 小的时刻

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];
	iota(ord,ord+n,0);
	sort(ord,ord+n,[](int u,int v){return a[u]<a[v];});	//高度两两不等，序唯一
	cin>>m;
	for(int x,y,k;m--;)
	{
		cin>>x>>y>>k;--x;--y;
		int cnt=0,ans=0;
		for(int v=0;v<n;++v)
		{
			if(ord[v]<x||ord[v]>y)
				continue;
			if(++cnt==k)
			{
				ans=ord[v];
				break;
			}
		}
		cout<<ans+1<<'\n';	//转回 1-based 时刻
	}
	return 0;
}
