//
// 题目：P1660 数位平方和
// https://www.luogu.com.cn/problem/P1660
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxv{3200005};
constexpr int mod{10000007};

int k,A,B,pw[10],nxt[mxv],H[mxv];
uint8_t st[mxv];

int calc_s(int x)
{
	int s=0;
	for(;x;x/=10)
		s+=pw[x%10];
	return s;
}

int get_nxt(int x)
{
	if(!nxt[x])
		nxt[x]=calc_s(x);
	return nxt[x];
}

int solve_u(int start)
{
	static vector<int>path;
	path.clear();
	int u=start;
	while(st[u]==0)
	{
		st[u]=1;
		path.push_back(u);
		u=get_nxt(u);
	}
	if(st[u]==1)
	{
		int pos=0;
		while(path[pos]!=u)
			++pos;
		int mn=u;
		for(int i=pos;i<(int)path.size();++i)
			mn=min(mn,path[i]);
		for(int i=pos;i<(int)path.size();++i)
		{
			H[path[i]]=mn;
			st[path[i]]=2;
		}
		for(int i=pos-1;i>=0;--i)
		{
			int x=path[i];
			H[x]=min(x,H[get_nxt(x)]);
			st[x]=2;
		}
	}
	else
	{
		for(int i=(int)path.size()-1;i>=0;--i)
		{
			int x=path[i];
			H[x]=min(x,H[get_nxt(x)]);
			st[x]=2;
		}
	}
	return H[start];
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>k>>A>>B;
	if(A>B)
		swap(A,B);
	for(int d=0;d<10;++d)
	{
		int r=1;
		for(int i=0;i<k;++i)
			r*=d;
		pw[d]=r;
	}

	i64 ans=0;
	for(int i=A;i<=B;++i)
	{
		int v=(st[i]==2)?H[i]:solve_u(i);
		ans=(ans+v)%mod;
	}
	cout<<ans<<'\n';
	return 0;
}
