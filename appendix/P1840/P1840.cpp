//
// 题目：P1840 Color the Axis
// https://www.luogu.com.cn/problem/P1840
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{200010};

int n,m,fa[mxn],black;

int find(int x)
{
	int r=x;
	while(fa[r]!=r)
		r=fa[r];
	while(x!=r)
	{
		int p=fa[x];
		fa[x]=r;
		x=p;
	}
	return r;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	for(int i=1;i<=n+1;++i)
		fa[i]=i;
	black=n;
	for(int i=0;i<m;++i)
	{
		int l,r;
		cin>>l>>r;
		for(int x=find(l);x<=r;x=find(x))
		{
			fa[x]=find(x+1);
			--black;
		}
		cout<<black<<'\n';
	}
	return 0;
}
