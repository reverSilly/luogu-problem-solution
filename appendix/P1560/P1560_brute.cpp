#include<bits/stdc++.h>
using namespace std;

int n,b,ans=1;
bool obs[125][125],vis[125][125];
int dr[4]={1,-1,0,0};
int dc[4]={0,0,1,-1};

void go(int r,int c,int d,int cur)
{
	ans=max(ans,cur);
	int nr=r+dr[d],nc=c+dc[d];
	if(nr>=0&&nr<n&&nc>=0&&nc<n&&!obs[nr][nc])
	{
		if(vis[nr][nc])
			return;
		vis[nr][nc]=true;
		go(nr,nc,d,cur+1);
		vis[nr][nc]=false;
	}
	else
	{
		for(int nd=0;nd<4;++nd)
			if((d<2)!=(nd<2))
			{
				int tr=r+dr[nd],tc=c+dc[nd];
				if(tr>=0&&tr<n&&tc>=0&&tc<n&&!obs[tr][tc]&&!vis[tr][tc])
				{
					vis[tr][tc]=true;
					go(tr,tc,nd,cur+1);
					vis[tr][tc]=false;
				}
			}
	}
}

int main()
{
	cin>>n>>b;
	for(int i=0;i<b;++i)
	{
		string s;
		cin>>s;
		int col=s[0]-'A';
		int row=stoi(s.substr(1))-1;
		obs[row][col]=true;
	}
	vis[0][0]=true;
	if(1<n&&!obs[1][0])
	{
		vis[1][0]=true;
		go(1,0,0,2);
		vis[1][0]=false;
	}
	if(1<n&&!obs[0][1])
	{
		vis[0][1]=true;
		go(0,1,2,2);
		vis[0][1]=false;
	}
	cout<<ans<<'\n';
	return 0;
}
