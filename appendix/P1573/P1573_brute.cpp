//
// P1573 暴力基准：状态压缩 BFS
// 每个元素记录它所在的栈(0..3),共 4^n 个状态;每栈自顶向下是递增序列,
// 故"集合"即"状态",可用四进制数编码。小 n 下与公式对拍。
#include<bits/stdc++.h>
using namespace std;

int n;
vector<int> dis;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	int total=1;
	for(int i=0;i<n;++i)
		total*=4;
	dis.assign(total,-1);
	int start=0;                       // 初始:全部元素在第四个栈(编号3)
	for(int i=0;i<n;++i)
		start|=3<<(2*i);
	int target=0;                      // 目标:全部元素在第一个栈(编号0)
	queue<int>Q;
	dis[start]=0;
	Q.push(start);
	while(!Q.empty())
	{
		int u=Q.front();
		Q.pop();
		if(u==target)
			break;
		int topv[4]={-1,-1,-1,-1};     // topv[s]=栈 s 的栈顶(元素编号最小者)
		for(int i=0;i<n;++i)
		{
			int s=(u>>(2*i))&3;
			if(topv[s]==-1)
				topv[s]=i;
		}
		for(int s=0;s<4;++s)
		{
			int x=topv[s];
			if(x==-1)
				continue;
			for(int d=0;d<4;++d)
			{
				if(d==s)
					continue;
				int y=topv[d];
				if(y==-1||x<y)         // 目标栈为空,或栈顶更大
				{
					int v=(u&~(3<<(2*x)))|(d<<(2*x));
					if(dis[v]==-1)
					{
						dis[v]=dis[u]+1;
						Q.push(v);
					}
				}
			}
		}
	}
	cout<<dis[target]%1000007<<'\n';
	return 0;
}
