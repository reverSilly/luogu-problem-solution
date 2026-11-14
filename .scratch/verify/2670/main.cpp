#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};
constexpr int dx[8]={-1,-1,-1,0,0,1,1,1},dy[8]={-1,0,1,-1,1,-1,0,1};

int n,m;
char g[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>g[i];
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<m;++j)
		{
			if(g[i][j]=='*')
			{
				cout<<'*';
				continue;
			}
			int cnt=0;
			for(int d=0;d<8;++d)
			{
				int x=i+dx[d],y=j+dy[d];
				if(x>=0&&x<n&&y>=0&&y<m&&g[x][y]=='*')
					++cnt;
			}
			cout<<char('0'+cnt);
		}
		cout<<'\n';
	}
	return 0;
}