#include<bits/stdc++.h>
using namespace std;

int n,m,k1,k2,tot,half_n,best=1e9;
int pr[60],pc[60];
bool vis[55][55];
int dr[4]={1,-1,0,0};
int dc[4]={0,0,1,-1};

void dfs(int step,int r,int c,int cur_max)
{
	if(cur_max>=best)
		return;
	pr[step]=r;pc[step]=c;
	if(step>half_n)
	{
		int w=k1*abs(r-pr[step-half_n])+k2*abs(c-pc[step-half_n]);
		cur_max=max(cur_max,w);
		if(cur_max>=best)
			return;
	}
	if(step==tot)
	{
		best=cur_max;
		return;
	}
	for(int d=0;d<4;++d)
	{
		int nr=r+dr[d],nc=c+dc[d];
		if(nr>=0&&nr<n&&nc>=0&&nc<m&&!vis[nr][nc])
		{
			vis[nr][nc]=true;
			dfs(step+1,nr,nc,cur_max);
			vis[nr][nc]=false;
		}
	}
}

int main()
{
	cin>>n>>m>>k1>>k2;
	tot=n*m;
	half_n=tot/2;
	vis[0][m-1]=true;
	dfs(1,0,m-1,0);
	cout<<best<<'\n';
	return 0;
}
