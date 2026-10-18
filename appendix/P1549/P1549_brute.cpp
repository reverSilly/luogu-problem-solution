#include<bits/stdc++.h>
using namespace std;

int n,best_sum;
int g[10][10],best_g[10][10];
bool used[105],is_p[205];

void dfs(int r,int c)
{
	if(r==n)
	{
		int s=1;
		for(int j=1;j<n;++j) s+=g[0][j];
		for(int i=1;i<n;++i) s+=g[i][0];
		if(s<best_sum)
		{
			best_sum=s;
			memcpy(best_g,g,sizeof(g));
		}
		return;
	}
	int nr=(c+1==n)?r+1:r;
	int nc=(c+1==n)?0:c+1;
	for(int v=2;v<=n*n;++v)
	{
		if(used[v]) continue;
		if(r>0&&!is_p[v+g[r-1][c]]) continue;
		if(c>0&&!is_p[v+g[r][c-1]]) continue;
		used[v]=true;
		g[r][c]=v;
		dfs(nr,nc);
		used[v]=false;
	}
}

int main()
{
	cin>>n;
	if(n<=1)
	{
		cout<<"NO\n";
		return 0;
	}
	for(int i=2;i<205;++i)
	{
		is_p[i]=true;
		for(int j=2;j*j<=i;++j)
			if(i%j==0)
				is_p[i]=false;
	}
	g[0][0]=1;
	used[1]=true;
	best_sum=1e9;
	dfs(0,1);
	if(best_sum>=1e9)
	{
		cout<<"NO\n";
		return 0;
	}
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cout<<best_g[i][j]<<(j+1==n?'\n':' ');
	return 0;
}
