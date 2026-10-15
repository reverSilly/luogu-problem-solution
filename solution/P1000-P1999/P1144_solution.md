# P1144 最短路计数

## 1. 重述问题

无向无权图，N 个顶点（N ≤ 10⁶），M 条边（M ≤ 2×10⁶，有自环和重边）。对每个顶点 i，统计从顶点 1 到 i 的最短路径条数，结果模 100003。若不可达则输出 0。

## 2. 推理链

### 2.1 无权图的最短路

每条边权重为 1，BFS 可以得到从源点到所有点的最短路长度（按层扩展天然保证首次访问的距离最小）。

### 2.2 最短路计数

设 `dis[v]` 为 1 到 v 的最短路长度，`cnt[v]` 为最短路径条数。

BFS 分层探索时，从 u 访问到 v：
- 如果 `dis[v]` 首次被设定（即 `dis[v]==-1` 未访问过），则 `dis[v]=dis[u]+1, cnt[v]=cnt[u]`
- 如果 `dis[v]==dis[u]+1`（即已有邻居以相同最短距离到达 v），则 `cnt[v]=(cnt[v]+cnt[u])%MOD`

**引理 1（BFS 的层序性质）**：BFS 按照与源点的距离严格递增的顺序访问所有顶点。当第一次访问到 v 时，`dis[v]` 被设置为 `dis[u]+1`，这个值就是 1 到 v 的最短距离。

**证明**：BFS 使用队列，初始状态距离为 0，每次从队首取 u，扩展其所有邻居。由于所有边权为 1，队列中的顶点距离单调非减，且同一距离层的顶点连续排列。因此第一次访问到 v 时的距离一定是最短的。□

**引理 2（计数正确性）**：上述转移公式正确统计了从 1 到 v 的最短路径条数。

**证明**：考虑任意一条 1 到 v 的最短路径，设其倒数第二个顶点为 u，则路径为 1→...→u→v，且 1→u 也是一条从 1 到 u 的最短路径。因此所有最短路径可以通过枚举倒数第二个顶点 u 来分类，只要 u 满足 (u,v) 是边且 `dis[u]+1 = dis[v]`。

在 BFS 中遍历 u 的所有邻居 v 时，如果 `dis[v] == dis[u]+1`，说明 u 是 v 的一个前驱。`cnt[v] += cnt[u]` 正确累加了经过 u 到达 v 的路径数。

每种分类互斥（以倒数第二顶点区分类别），且覆盖全部情况。因此求和正确。□

### 2.3 重边与自环的处理

- **自环**：自环 (v,v) 不会产生新的最短路，因为 `dis[v]+1 > dis[v]`，自环上的路径不会是最短的。因此可以忽略。
- **重边**：重边不影响 `cnt` 的计算，因为 BFS 会处理每条边。如果 u 和 v 之间有多条边，但 `dis[u]+1 == dis[v]` 时，每次遇到都会累加一次 cnt[u]，相当于每条重边都算一条不同路径。

### 2.4 复杂度

N ≤ 10⁶，M ≤ 2×10⁶，BFS O(N+M) 可过。注意内存使用：邻接表用 vector<int> 总边数约 4×10⁶ 个 int。

## 3. 模式总结

- **无权图 BFS 最短路** — BFS 天然按距离分层
- **BFS 上 DP 计数** — 首次访问继承 cnt，同层访问累加 cnt
- **模运算** — 中间结果取模防溢出

## 4. 真实解法

### 符号表

| 符号 | 含义 |
|------|------|
| `N, M` | 顶点数、边数 |
| `graph[v]` | v 的邻接表 |
| `dis[v]` | 1 到 v 的最短距离，初始 -1（未访问） |
| `cnt[v]` | 最短路径条数，模 100003 |
| `MOD` | 100003 |
| `q` | BFS 队列 |

### 算法步骤

1. 读入 N, M，建邻接表
2. `dis[1]=0; cnt[1]=1;` BFS 从 1 开始
3. 每次取出 u，遍历 graph[u]：

   ```
   if dis[v] == -1:
       dis[v] = dis[u] + 1
       cnt[v] = cnt[u]
       q.push(v)
   else if dis[v] == dis[u] + 1:
       cnt[v] = (cnt[v] + cnt[u]) % MOD
   ```
4. 输出 cnt[1]..cnt[N]（每行一个）

### 复杂度

O(N + M)，空间 O(N + M)。

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1000005};
constexpr int MOD{100003};

int N,M,dis[mxn],cnt[mxn];
vector<int>graph[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>N>>M;
	for(int i=0,u,v;i<M;++i)
	{
		cin>>u>>v;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}

	fill_n(dis+1,N,-1);
	queue<int>q;
	dis[1]=0;
	cnt[1]=1;
	q.push(1);
	while(!q.empty())
	{
		int u=q.front();q.pop();
		for(int v:graph[u])
		{
			if(dis[v]==-1)
			{
				dis[v]=dis[u]+1;
				cnt[v]=cnt[u];
				q.push(v);
			}
			else if(dis[v]==dis[u]+1)
			{
				cnt[v]=(cnt[v]+cnt[u])%MOD;
			}
		}
	}

	for(int i=1;i<=N;++i)
		cout<<cnt[i]<<'\n';

	return 0;
}
```
