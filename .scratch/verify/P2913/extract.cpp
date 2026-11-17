// 题目：P2913 [USACO08OCT] Wheel Rotation G
// 转向 0/1 编码：直连 XOR 0，交叉 XOR 1 ⇒ dir[v]=dir[u]^c。
// N 点 N-1 边连通 ⇒ 树，路径唯一 ⇒ 从滑轮 1 BFS 传播即可。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005},inf{1145141919};

int n,dir[mxn];
vector<pair<int,int>>graph[mxn];	// {邻居,连接类型}，无向

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	fill_n(dir,n,inf);	// 全局默认 0 是合法方向！未访问必须显式置 inf
	for(int i=1,s,d,c;i<n;++i)
	{
		cin>>s>>d>>c;--s;--d;
		graph[s].push_back({d,c});
		graph[d].push_back({s,c});
	}
	queue<int>Q;
	dir[0]=0;Q.push(0);	// 滑轮 1 顺时针
	while(!Q.empty())
	{
		int u=Q.front();Q.pop();
		for(auto[v,c]:graph[u])
			if(dir[v]==inf)
			{
				dir[v]=dir[u]^c;
				Q.push(v);
			}
	}
	cout<<dir[n-1]<<'\n';
	return 0;
}
