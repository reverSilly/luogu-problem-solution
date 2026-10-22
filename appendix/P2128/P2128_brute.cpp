#include<bits/stdc++.h>
using namespace std;

//暴力：枚举点集子集，判断是否为团（两两有铁索），取价值总和最大者
constexpr int mxn{25};

int n,m,val[mxn];
bool adj[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>val[i];
	for(int i=0;i<m;++i)
	{
		int a,b;
		cin>>a>>b;
		--a;--b;
		adj[a][b]=adj[b][a]=true;
	}
	int ans=0;
	for(int s=0;s<(1<<n);++s)
	{
		bool ok=true;
		int w=0;
		for(int i=0;i<n&&ok;++i)
		{
			if(!(s>>i&1))continue;
			w+=val[i];
			for(int j=i+1;j<n;++j)
				if((s>>j&1)&&!adj[i][j])
				{
					ok=false;
					break;
				}
		}
		if(ok)ans=max(ans,w);
	}
	cout<<ans<<'\n';
	return 0;
}
