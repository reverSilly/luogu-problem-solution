#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

int n,m;
//a[i]：第 i 个路口与第 i+1 个路口的距离
int a[mxn];
//r[i],g[i]：红灯、绿灯时间
int r[mxn],g[mxn];

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

	//t=0 时所有路口刚由红转绿，故一个周期内前 g[i] 分钟是绿灯
	i64 t=m;
	for(int i=1;i<=n;++i)
	{
		if(i>1)
			t+=a[i-1];
		i64 per=r[i]+g[i];
		i64 ph=t%per;
		if(ph>=g[i])
			t+=per-ph;
		cout<<t<<'\n';
	}
	return 0;
}
