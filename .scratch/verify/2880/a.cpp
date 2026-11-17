// 题目：P2880 [USACO07JAN] Balanced Lineup G
// 静态区间 max-min ⇒ 双 ST 表；max/min 幂等 ⇒ 两块重叠覆盖 O(1) 查询
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{50005},mxk{17};

int n,q,h[mxn],lg[mxn],stmx[mxk][mxn],stmn[mxk][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>q;
	for(int i=0;i<n;++i)
	{
		cin>>h[i];
		stmx[0][i]=stmn[0][i]=h[i];
	}
	for(int i=2;i<=n;++i)
		lg[i]=lg[i>>1]+1;
	for(int k=1;(1<<k)<=n;++k)
		for(int i=0;i+(1<<k)<=n;++i)
		{
			stmx[k][i]=max(stmx[k-1][i],stmx[k-1][i+(1<<(k-1))]);
			stmn[k][i]=min(stmn[k-1][i],stmn[k-1][i+(1<<(k-1))]);
		}
	for(int i=0,a,b;i<q;++i)
	{
		cin>>a>>b;--a;--b;
		if(a>b)
			swap(a,b);
		int k=lg[b-a+1];
		int mx=max(stmx[k][a],stmx[k][b-(1<<k)+1]);
		int mn=min(stmn[k][a],stmn[k][b-(1<<k)+1]);
		cout<<mx-mn<<'\n';
	}
	return 0;
}
