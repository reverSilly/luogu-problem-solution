// 题目：P2996 [USACO10NOV] Visiting Cows G
// 树上最大独立集：dp[u][0/1] = 不选/选 u 时子树内最多拜访数
// 链状数据深度可达 5e4，用 BFS 序倒序递推代替递归，杜绝爆栈
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{50005};

int n,dp[mxn][2],par[mxn],ord[mxn];
vector<int>graph[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1,u,v;i<n;++i)
	{
		cin>>u>>v;--u;--v;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}
	int tail=0;
	ord[tail++]=0;par[0]=-1;
	for(int head=0;head<tail;++head)	// ord 数组本身当 BFS 队列
	{
		int u=ord[head];
		for(int v:graph[u])
			if(v!=par[u])
			{
				par[v]=u;
				ord[tail++]=v;
			}
	}
	for(int i=n-1;i>=0;--i)	// 逆 BFS 序：儿子一定先于父亲被递推
	{
		int u=ord[i];
		dp[u][1]=1;	// dp 是计数不是距离，全局 0 初值恰好合法
		for(int v:graph[u])
			if(v!=par[u])
			{
				dp[u][0]+=max(dp[v][0],dp[v][1]);
				dp[u][1]+=dp[v][0];
			}
	}
	cout<<max(dp[0][0],dp[0][1])<<'\n';
	return 0;
}