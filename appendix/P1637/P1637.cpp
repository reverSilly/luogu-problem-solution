//
// 题目：P1637 三元上升子序列
// https://www.luogu.com.cn/problem/P1637
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{30005},mxv{100005};

int n,a[mxn],L[mxn],R[mxn],BIT[mxv];

int lowbit(int x){return x&-x;}

void add(int x,int v)
{
	for(;x<mxv;x+=lowbit(x))
		BIT[x]+=v;
}

int sigma(int x)
{
	int r=0;
	for(;x>0;x-=lowbit(x))
		r+=BIT[x];
	return r;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	int mv=0;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];
		mv=max(mv,a[i]);
	}

	for(int i=0;i<n;++i)
	{
		L[i]=sigma(a[i]-1);
		add(a[i],1);
	}

	fill_n(BIT,mv+2,0);
	for(int i=n-1;i>=0;--i)
	{
		R[i]=(n-1-i)-sigma(a[i]);
		add(a[i],1);
	}

	i64 ans=0;
	for(int i=0;i<n;++i)
		ans+=1LL*L[i]*R[i];
	cout<<ans<<'\n';
	return 0;
}
