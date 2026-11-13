#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25};

int m,side,a[mxn],cap[4];

bool dfs(int idx)
{
	if(idx==m)
		return true;
	int mn=a[m-1];					// 剩余木棒中最短的一根
	for(int k=0;k<4;++k)
	{
		int r=side-cap[k];
		if(r>0&&r<mn)				// 缺口填不满
			return false;
	}
	int s=a[idx],last=-1;
	for(int k=0;k<4;++k)
	{
		if(cap[k]==last)
			continue;
		if(cap[k]+s>side)
			continue;
		last=cap[k];
		cap[k]+=s;
		if(dfs(idx+1))
			return true;
		cap[k]-=s;
		if(cap[k]==0)
			return false;
	}
	return false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int n;
	cin>>n;
	while(n--)
	{
		cin>>m;
		int tot=0,mx=0;
		for(int i=0;i<m;++i)
		{
			cin>>a[i];
			tot+=a[i];
			mx=max(mx,a[i]);
		}
		if(tot%4!=0)
		{
			cout<<"no\n";
			continue;
		}
		side=tot/4;
		if(mx>side)
		{
			cout<<"no\n";
			continue;
		}
		sort(a,a+m,greater<int>());
		cap[0]=cap[1]=cap[2]=cap[3]=0;
		cout<<(dfs(0)?"yes\n":"no\n");
	}
	return 0;
}
