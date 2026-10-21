//
// 题目：P1816 忠诚
// https://www.luogu.com.cn/problem/P1816
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};
constexpr int mxk{18};

int m,n;
int st[mxn][mxk];
int lg[mxn];

int qry(int l,int r)
{
	int k{lg[r-l+1]};
	return min(st[l][k],st[r-(1<<k)+1][k]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n;
	lg[1]=0;
	for(int i=2;i<=m;++i)
		lg[i]=lg[i>>1]+1;
	for(int i=1;i<=m;++i)
		cin>>st[i][0];
	for(int k=1;k<mxk;++k)
		for(int i=1;i+(1<<k)-1<=m;++i)
			st[i][k]=min(st[i][k-1],st[i+(1<<(k-1))][k-1]);
	for(int i=0;i<n;++i)
	{
		int a,b;
		cin>>a>>b;
		if(a>b)
			swap(a,b);
		if(i)
			cout<<' ';
		cout<<qry(a,b);
	}
	cout<<'\n';
	return 0;
}
