#include<bits/stdc++.h>
using namespace std;

constexpr int mod=10007;
int n,m,ans;
vector<string> g;
int a[230][230];

void dfs(int r,int c)
{
	if(r==n)
	{
		ans=(ans+1)%mod;
		return;
	}
	int nr=c+1==m?r+1:r;
	int nc=c+1==m?0:c+1;
	if(g[r][c]!='1')
	{
		a[r][c]=0;
		dfs(nr,nc);
	}
	if(g[r][c]!='0')
	{
		if((r==0||a[r-1][c]==0)&&(c==0||a[r][c-1]==0))
		{
			a[r][c]=1;
			dfs(nr,nc);
		}
	}
}

int main()
{
	cin>>n>>m;
	g.resize(n);
	for(int i=0;i<n;++i)
		cin>>g[i];
	dfs(0,0);
	cout<<ans<<'\n';
	return 0;
}
