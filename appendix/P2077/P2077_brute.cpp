#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int n,m;
int a[mxn],r[mxn],g[mxn];

//模拟第 i 个路口的灯：返回不早于 T 的第一个绿灯时刻
int waitgreen(int i,int T)
{
	int rem=g[i];
	bool green=true;
	int t=0;
	while(true)
	{
		if(t>=T&&green)
			return t;
		if(green)
		{
			--rem;
			if(rem==0)
			{
				green=false;
				rem=r[i];
			}
		}
		else
		{
			--rem;
			if(rem==0)
			{
				green=true;
				rem=g[i];
			}
		}
		++t;
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=1;i<n;++i)
		cin>>a[i];
	for(int i=1;i<=n;++i)
		cin>>r[i];
	for(int i=1;i<=n;++i)
		cin>>g[i];

	//暴力：一分钟一分钟地推时间
	int t=m;
	for(int i=1;i<=n;++i)
	{
		if(i>1)
			t+=a[i-1];
		t=waitgreen(i,t);
		cout<<t<<'\n';
	}
	return 0;
}
