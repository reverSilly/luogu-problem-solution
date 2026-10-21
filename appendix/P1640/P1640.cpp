//
// 题目：P1640 [SCOI2010] 连续攻击游戏
// https://www.luogu.com.cn/problem/P1640
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxv{10005};

int n,f[mxv],mx[mxv];
bool cyc[mxv];

int getrt(int x)
{
	if(f[x]==x)
		return x;
	return f[x]=getrt(f[x]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	for(int i=1;i<mxv;++i)
	{
		f[i]=i;
		mx[i]=i;
		cyc[i]=false;
	}

	cin>>n;
	for(int i=0,u,v;i<n;++i)
	{
		cin>>u>>v;
		int ru=getrt(u),rv=getrt(v);
		if(ru==rv)
			cyc[ru]=true;
		else
		{
			f[ru]=rv;
			mx[rv]=max(mx[rv],mx[ru]);
			cyc[rv]=cyc[rv]||cyc[ru];
		}
	}

	int ans=0;
	for(int i=1;i<mxv;++i)
	{
		int r=getrt(i);
		if(cyc[r]||i<mx[r])
			ans=i;
		else
			break;
	}
	cout<<ans<<'\n';
	return 0;
}
