// 题目：P2958 [USACO09OCT] Papaya Jungle G
// 纯模拟：吃光当前格（置零），走向四邻中未吃木瓜最多的格子，直到 (R,C)
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{45};
constexpr int dx[4]{-1,1,0,0},dy[4]{0,0,-1,1};

int r,c,a[mxn][mxn],ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>r>>c;
	for(int i=0;i<r;++i)
		for(int j=0;j<c;++j)
			cin>>a[i][j];
	int x=0,y=0;
	while(true)
	{
		ans+=a[x][y];
		a[x][y]=0;		// 吃光即置零，重访不再贡献
		if(x==r-1&&y==c-1)
			break;
		int bx=-1,by=-1,best=-1;
		for(int d=0;d<4;++d)
		{
			int nx=x+dx[d],ny=y+dy[d];
			if(nx<0||nx>=r||ny<0||ny>=c)
				continue;
			if(a[nx][ny]>best)
				best=a[nx][ny],bx=nx,by=ny;
		}
		x=bx;y=by;		// 题目保证最大值唯一且过程终于 (R,C)
	}
	cout<<ans<<'\n';
	return 0;
}
