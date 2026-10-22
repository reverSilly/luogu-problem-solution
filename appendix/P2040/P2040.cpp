// 题目：P2040 打开所有的灯
// 思路：把 9 盏灯压成 9 位状态，状态图上 BFS 求最短路
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{520};
constexpr int inf{1145141919};

int tg[9];				// tg[i]: 点击第 i 个位置会翻转的位掩码
int dis[1<<9];			// dis[state]: 从初始状态到 state 的最少点击数

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int st=0;
	for(int i=0;i<9;++i)
	{
		int x;
		cin>>x;
		if(x) st|=1<<i;		// 1 表示亮
	}
	// 预处理每个位置自身的翻转掩码：自己 + 上下左右
	for(int i=0;i<9;++i)
	{
		int r=i/3,c=i%3,mask=1<<i;
		if(r>0) mask|=1<<(i-3);
		if(r<2) mask|=1<<(i+3);
		if(c>0) mask|=1<<(i-1);
		if(c<2) mask|=1<<(i+1);
		tg[i]=mask;
	}
	fill_n(dis,1<<9,inf);
	dis[st]=0;
	queue<int>q;
	q.push(st);
	while(!q.empty())
	{
		int u=q.front();
		q.pop();
		for(int i=0;i<9;++i)
		{
			int v=u^tg[i];		// 点击等价于异或翻转
			if(dis[v]==inf)
			{
				dis[v]=dis[u]+1;
				q.push(v);
			}
		}
	}
	// 全部点亮对应 9 位全 1
	if(dis[(1<<9)-1]==inf)
		cout<<-1<<'\n';
	else
		cout<<dis[(1<<9)-1]<<'\n';
	return 0;
}
