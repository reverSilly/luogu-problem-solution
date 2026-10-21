//
// 题目：P1865 A % B Problem
// https://www.luogu.com.cn/problem/P1865
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1000005};

int n,m;
int pr[mxn],pc;
bool vis[mxn];
int sf[mxn];

void sieve()
{
	vis[1]=true;
	for(int i=2;i<=m;++i)
	{
		if(!vis[i])
			pr[pc++]=i;
		for(int j=0;j<pc;++j)
		{
			i64 v=1ll*pr[j]*i;
			if(v>m)
				break;
			vis[v]=true;
			if(i%pr[j]==0)
				break;
		}
	}
	for(int i=1;i<=m;++i)
		sf[i]=sf[i-1]+(!vis[i]?1:0);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	sieve();
	for(int i=0;i<n;++i)
	{
		i64 l,r;
		cin>>l>>r;
		if(l<1||r>m)
			cout<<"Crossing the line\n";
		else
			cout<<sf[(int)r]-sf[(int)l-1]<<'\n';
	}
	return 0;
}
