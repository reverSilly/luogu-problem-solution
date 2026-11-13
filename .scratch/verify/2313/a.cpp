// 题目：P2313 [HNOI2005] 汤姆的游戏
// N,M<=500 => 每个点枚举每个图形，2.5e5 次判定，难点全在判定本身：
//   矩形：给的是一对「对角」顶点，顺序任意 => 先归一成 min/max 两个区间
//   圆  ：平方距离比较，不开方
//   边界不算内部 => 一律严格不等号，禁止 eps（加 eps 会把边界算进去，方向反了）
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{505};

int n,m;
char ty[mxn];			//'r' 矩形 / 'c' 圆
long double g[mxn][4];	//矩形：g[0],g[1] 是 x 的下界/上界，g[2],g[3] 是 y 的下界/上界；圆：g[0],g[1] 圆心，g[2] 半径

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
	{
		cin>>ty[i];
		if(ty[i]=='r')
		{
			long double x1,y1,x2,y2;
			cin>>x1>>y1>>x2>>y2;
			g[i][0]=min(x1,x2);g[i][1]=max(x1,x2);
			g[i][2]=min(y1,y2);g[i][3]=max(y1,y2);
		}
		else
			cin>>g[i][0]>>g[i][1]>>g[i][2];
	}
	for(int j=0;j<m;++j)
	{
		long double x,y;
		cin>>x>>y;
		int ans=0;
		for(int i=0;i<n;++i)
		{
			if(ty[i]=='r')
				ans+=g[i][0]<x&&x<g[i][1]&&g[i][2]<y&&y<g[i][3];
			else
			{
				long double dx=x-g[i][0],dy=y-g[i][1];
				ans+=dx*dx+dy*dy<g[i][2]*g[i][2];	//严格小于：落在圆周上不算内部
			}
		}
		cout<<ans<<'\n';
	}
	return 0;
}
