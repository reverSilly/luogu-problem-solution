//
// P1596 朴素解：并查集, 每个 W 与右/下/右下/左下四邻合并, 统计不同根数
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int n,m;
char g[mxn][mxn];
int f[mxn*mxn];

int getrt(int x)
{
	return f[x]==x?x:f[x]=getrt(f[x]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>g[i];
	for(int i=0;i<n*m;++i)
		f[i]=i;
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
		{
			if(g[i][j]!='W')
				continue;
			int dx[4]{0,1,1,1},dy[4]{1,0,1,-1};   // 右/下/右下/左下, 避免重复
			for(int k=0;k<4;++k)
			{
				int x=i+dx[k],y=j+dy[k];
				if(x<n&&y>=0&&y<m&&g[x][y]=='W')
					f[getrt(i*m+j)]=getrt(x*m+y);
			}
		}
	set<int>s;
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
			if(g[i][j]=='W')
				s.insert(getrt(i*m+j));
	cout<<s.size()<<'\n';
	return 0;
}
