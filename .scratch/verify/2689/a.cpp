// 题目：P2689 东南西北
// E/W 只改 x，N/S 只改 y ⇒ 两轴解耦，各自是一维计数问题
// 下界 |dx|+|dy|（每步只改一个坐标 1），所需方向风次数够时可构造达到下界
// 于是答案就是曼哈顿距离，或者 -1；不需要搜索，也不需要假设坐标范围
#include<bits/stdc++.h>
using namespace std;

constexpr int mxc{128};	// ASCII 计数桶

int sx,sy,tx,ty,t,cnt[mxc];	// 别用 y1 当变量名：与数学库贝塞尔函数冲突

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>sx>>sy>>tx>>ty>>t;
	for(int i=0;i<t;++i)
	{
		char c;cin>>c;	// 每行一个字符，>> 自动跳过换行
		++cnt[(int)c];
	}
	int dx=tx-sx,dy=ty-sy;
	int nx=dx>0?cnt['E']:cnt['W'];	// dx==0 归到 W 分支，需求 0 恒满足
	int ny=dy>0?cnt['N']:cnt['S'];
	if(nx<abs(dx)||ny<abs(dy))
		cout<<-1<<'\n';
	else
		cout<<abs(dx)+abs(dy)<<'\n';
	return 0;
}
