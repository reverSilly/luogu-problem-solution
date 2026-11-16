// 题目：P2771 [USACO16JAN] Build Gates S
// 围栏是连通平面图：V 个不同格点、E 条不同单位线段。
// Euler 公式 V-E+F=2 ⇒ 区域数 F=E-V+2；每扇门至多合并两个区域 ⇒ 答案 F-1=E-V+1。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{2005},ofs{1001};	// 坐标 ∈[-1000,1000]，平移 ofs 后 ∈[1,2001]

int n,vcnt,ecnt;
bool vis[mxn][mxn],eh[mxn][mxn],ev[mxn][mxn];
// eh[x][y]：横线段 (x,y)-(x+1,y)；ev[x][y]：竖线段 (x,y)-(x,y+1)；都记在较小端点
string s;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>s;
	int x=ofs,y=ofs;
	vis[x][y]=true;vcnt=1;
	for(char c:s)
	{
		int nx=x,ny=y;
		if(c=='N')++ny;
		else if(c=='S')--ny;
		else if(c=='E')++nx;
		else --nx;
		if(!vis[nx][ny])
		{
			vis[nx][ny]=true;
			++vcnt;
		}
		if(nx==x)	// 竖线段
		{
			int yy=min(y,ny);
			if(!ev[x][yy])
			{
				ev[x][yy]=true;
				++ecnt;
			}
		}
		else	// 横线段
		{
			int xx=min(x,nx);
			if(!eh[xx][y])
			{
				eh[xx][y]=true;
				++ecnt;
			}
		}
		x=nx;y=ny;
	}
	cout<<ecnt-vcnt+1<<'\n';
	return 0;
}