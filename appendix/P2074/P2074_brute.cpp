#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{55};

int n,m,k,t;
int bx[mxn],by[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>k>>t;
	for(int i=1;i<=k;++i)
		cin>>bx[i]>>by[i];

	//暴力：把整个网格的每个格点都扫一遍，用浮点开方判断是否在圆内
	int ans=0;
	for(int i=1;i<=k;++i)
	{
		int cnt=0;
		for(int x=1;x<=n;++x)
			for(int y=1;y<=m;++y)
			{
				double dx=x-bx[i];
				double dy=y-by[i];
				if(sqrt(dx*dx+dy*dy)<=t+1e-9)
					++cnt;
			}
		ans=max(ans,cnt);
	}
	cout<<ans<<'\n';
	return 0;
}
