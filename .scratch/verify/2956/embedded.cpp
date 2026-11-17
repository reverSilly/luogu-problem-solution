// 题目：P2956 [USACO09OCT] The Robot Plow G
// 矩形并的格子数：直接打标记。bool 标记天然去重，重叠不会重复计数
// 坐标含两端：矩形覆盖 (x2-x1+1)*(y2-y1+1) 格，读入后转 0-based
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{245};

int X,Y,I,ans;
bool g[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>X>>Y>>I;
	for(int i=0,x1,y1,x2,y2;i<I;++i)
	{
		cin>>x1>>y1>>x2>>y2;--x1;--y1;--x2;--y2;
		for(int x=x1;x<=x2;++x)
			for(int y=y1;y<=y2;++y)
				g[x][y]=true;
	}
	for(int x=0;x<X;++x)
		for(int y=0;y<Y;++y)
			ans+=g[x][y];
	cout<<ans<<'\n';
	return 0;
}
