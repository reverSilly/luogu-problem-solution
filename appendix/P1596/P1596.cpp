//
// 题目：P1596 [USACO10OCT] Lake Counting S
// https://www.luogu.com.cn/problem/P1596
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int n,m,ans;
char g[mxn][mxn];
bool vis[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>g[i];

	int qx[mxn*mxn],qy[mxn*mxn];
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
		{
			if(g[i][j]!='W'||vis[i][j])
				continue;
			++ans;
			int ql=0,qr=0;
			vis[i][j]=true;
			qx[qr]=i;qy[qr]=j;++qr;
			while(ql<qr)
			{
				int x=qx[ql],y=qy[ql];
				++ql;
				for(int dx=-1;dx<=1;++dx)
					for(int dy=-1;dy<=1;++dy)
					{
						int nx=x+dx,ny=y+dy;
						if(nx<0||nx>=n||ny<0||ny>=m)
							continue;
						if(g[nx][ny]=='W'&&!vis[nx][ny])
						{
							vis[nx][ny]=true;
							qx[qr]=nx;qy[qr]=ny;++qr;
						}
					}
			}
		}
	cout<<ans<<'\n';
	return 0;
}
