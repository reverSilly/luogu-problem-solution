//
// P1612 暴力（对拍用）：对每个点沿父指针一路上跳累加权值, 直到超过 c_u
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int n,p[300005],w[300005],c[300005];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	p[0]=0;
	for(int i=1;i<n;++i)
	{
		cin>>p[i];
		--p[i];
	}
	for(int i=0;i<n;++i)
		cin>>w[i];
	for(int i=0;i<n;++i)
		cin>>c[i];
	for(int u=0;u<n;++u)
	{
		i64 sum=0;
		int len=0,x=u;
		while(true)
		{
			sum+=w[x];
			if(sum>c[u])
				break;
			++len;
			if(x==0)
				break;
			x=p[x];
		}
		cout<<len<<(u+1==n?'\n':' ');
	}
	return 0;
}
