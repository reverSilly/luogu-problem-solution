# P1137 旅行计划 题解

## 1. 重新读题

$N$ 个城市 $M$ 条有向边，边 $x\to y$ 表示 $x$ 在 $y$ 西面。从任意城市出发向东走到 $i$ 停止，求以 $i$ 为终点的最长路径长度（城市数）。$N\le 10^5,\;M\le 2\times10^5$。

## 2. 思维链

### 2.1 图的性质

东面关系有传递性：$x\to y$ 意味着始终向东走，不会出现环。因此图是 **DAG**。

### 2.2 DP 状态

定义 $dp[i]$ = 以 $i$ 为终点的最多城市数。

$$
dp[i] = 1 + \max_{j\to i} dp[j]
$$

起点 $dp[i]=1$（只游览自己）。

### 2.3 计算顺序

DAG 上按拓扑序递推即可保证转移时 $dp[j]$ 已计算完毕。

**引理 1**：按拓扑序处理，转移 $dp[i] = 1 + \max_{j\in pre[i]} dp[j]$ 可在 $O(N+M)$ 内完成。

**证明**：拓扑排序后每个前驱 $j$ 都在 $i$ 之前访问，$dp[j]$ 已确定，每条边恰好被枚举一次。

### 2.4 边界

- 无入度的城市：$dp[i]=1$
- 答案可能为 1（无东向边可达）

## 3. 模式总结

| 模式 | 说明 |
|------|------|
| DAG 最长路 | 按拓扑序 DP，$dp[v] = \max(dp[v], dp[u]+1)$ |
| 起点任意但终点固定 | 边方向与 DP 方向一致（入边） |

## 4. 正解

### 符号表

| 符号 | 含义 |
|------|------|
| $n,m$ | 城市数、道路数 |
| $ind[i]$ | 入度 |
| $dp[i]$ | 以 $i$ 为终点的最多城市数 |
| $\text{graph}[i]$ | $i$ 的邻接表（出边） |
| $q$ | 拓扑排序队列 |

### 算法步骤

1. 读入 $n,m$，建图并统计入度
2. 入度为 0 的点入队，$dp[i]=1$
3. 队列非空：取 $u$，遍历出边 $u\to v$；$ind[v]\gets ind[v]-1$；$dp[v]=\max(dp[v],dp[u]+1)$；若 $ind[v]=0$ 则入队
4. 输出 $dp[1..n]$ 每行一个

时间复杂度 $O(N+M)$，空间 $O(N+M)$。

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

int n,m,ind[mxn],dp[mxn];
vector<int>graph[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	for(int i=0;i<m;i++)
	{
		int u,v;cin>>u>>v;
		graph[--u].push_back(--v);
		++ind[v];
	}
	queue<int>q;
	for(int i=0;i<n;i++)
		if(!ind[i])
		{
			q.push(i);
			dp[i]=1;
		}
	while(!q.empty())
	{
		int u=q.front();q.pop();
		for(int v:graph[u])
		{
			if(dp[v]<dp[u]+1)
				dp[v]=dp[u]+1;
			if(!--ind[v])
				q.push(v);
		}
	}
	for(int i=0;i<n;i++)
		cout<<dp[i]<<'\n';
	return 0;
}
```
