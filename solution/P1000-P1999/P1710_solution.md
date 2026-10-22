# P1710 地铁涨价

## 1. 重新读题

给定一个 $N$ 个点、$M$ 条边的无向连通图（$N \le 10^5$，$M \le 2 \times 10^5$），学校位于 $1$ 号点。初始时每条边的票价均为 $1$ 元，每个车站 $u$ 的初始最低通勤费用等于 $1$ 号点到 $u$ 的无权最短路长度 $d_0(u)$。

共有 $Q$ 次永久性涨价操作（$Q \le M$），每次指定一条尚未涨价的边，将其票价从 $1$ 元改为 $2$ 元。当某个车站 $u$ 到 $1$ 号点的当前最短路费用严格大于初始费用 $d_0(u)$ 时，该站学生就会“不满”。对于每次涨价操作，输出当前不满的车站总数。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：涨到 $2$ 元的边还能用在“原价最短路”上吗？

表面上看，这是一道“动态修改边权求全源到单汇最短路”的问题；但每次从 $1$ 号点跑 Dijkstra 需要 $O(Q M \log N)$，完全不可行。

让我们抓住最核心的性质：学生关心的是**当前最短路是否仍然等于初始最短路 $d_0(u)$**，而不是涨价后具体变成了多少！

**引理 1（涨价边等价于删除、非最短路边永久无效）**：
在任意时刻，车站 $u$ 的最短通勤费用等于初始值 $d_0(u)$，当且仅当在原图中存在一条从 $1$ 到 $u$、长度为 $d_0(u)$ 且**全由未涨价（权值为 $1$）的边组成**的路径。

**证明**：
- **充分性**：若存在这样一条由 $d_0(u)$ 条权值为 $1$ 的边构成的路径，其总费用为 $d_0(u)$；而任何时刻边权均 $\ge 1$，不可能有比 $d_0(u)$ 更短的路径，故当前最短路恰为 $d_0(u)$。
- **必要性**：任取原图中从 $1$ 到 $u$ 的一条路径 $P$，设其包含 $k$ 条边。由初始无权最短路的定义，必有 $k \ge d_0(u)$。
  - 若 $k > d_0(u)$，即便 $P$ 上所有边都未涨价，$P$ 的费用至少为 $k \ge d_0(u) + 1 > d_0(u)$；
  - 若 $k = d_0(u)$ 但 $P$ 上至少有一条边已涨价（权值为 $2$），则 $P$ 的费用至少为 $(k - 1) \times 1 + 2 = d_0(u) + 1 > d_0(u)$。
  因此，只要某条路径的费用仍等于 $d_0(u)$，它就必须恰好由 $d_0(u)$ 条边组成，且每条边都未涨价。$\blacksquare$

### 2.2 提取最短路 DAG 与级联失效（附完整证明）

由引理 1，一条路径 $1 = v_0 \to v_1 \to \dots \to v_k = u$ 长度为 $d_0(u)$，当且仅当对每一步 $i$ 都有 $d_0(v_{i+1}) = d_0(v_i) + 1$。

据此，我们先从 $1$ 号点跑一次 BFS 求出所有点的 $d_0(u)$，然后只保留满足 $|d_0(u) - d_0(v)| = 1$ 的边，并将其定向为从 $d_0$ 较小者指向较大者，得到**最短路有向无环图（Shortest-Path DAG）** $G_{\text{DAG}}$。
- 凡是不满足 $|d_0(u) - d_0(v)| = 1$ 的原图边，从一开始就不属于 $G_{\text{DAG}}$，无论它涨不涨价都对任何点的原价最短路毫无影响，直接忽略！
- 当 $G_{\text{DAG}}$ 中的一条有向边 $e = (u \to v)$ 涨价时，由引理 1，这等价于从 $G_{\text{DAG}}$ 中**删去有向边 $e$**。

但删去一条边后，怎么快速维护哪些点从 $1$ 号点不可达了？

**引理 2（DAG 有效入度判据与级联删除）**：
在 $G_{\text{DAG}}$ 中，称一个点 $u$ 是“存活的”，如果存在从 $1$ 到 $u$ 的未删除有向路径；称一条未删除的有向边 $e = (u \to v)$ 是“有效的”，如果其起点 $u$ 是存活的。对每个点 $v \ne 1$，维护其当前有效入边的条数 $\text{indeg}[v]$。则：
1. 点 $v \ne 1$ 存活当且仅当 $\text{indeg}[v] \ge 1$；
2. 当某条有效边 $e = (u \to v)$ 被外部涨价删除，或因起点 $u$ 刚刚变为不存活而失效时，令 $\text{indeg}[v]$ 减 $1$；一旦 $\text{indeg}[v]$ 变为 $0$，点 $v$ 立即变为不存活（学生开始不满），且从 $v$ 出发的所有未删除出边也随之失效，可递归触发下游更新。

**证明**：
对节点按拓扑序（即 $d_0(v)$ 从小到大）归纳：
- 基例 $d_0 = 0$：唯一节点为 $1$ 号点，恒存活。
- 归纳步：设所有 $d_0(u) < d_0(v)$ 的点 $u$ 的存活性已被正确维护。点 $v$ 存在从 $1$ 出发的未删除路径，当且仅当存在至少一条未删除的入边 $(u \to v)$ 使得起点 $u$ 存在从 $1$ 出发的未删除路径（即 $u$ 存活）。由于 $G_{\text{DAG}}$ 中所有入边 $(u \to v)$ 均满足 $d_0(u) = d_0(v) - 1 < d_0(v)$，$v$ 存活当且仅当它至少有一条有效入边，即 $\text{indeg}[v] \ge 1$。
由于边只会单向从“有效”变为“失效”、点只会单向从“存活”变为“不存活”，每当 $\text{indeg}[v]$ 降为 $0$ 时将其所有出边标记失效并递归扣减下游入度，即可不重不漏地维护所有点的存活状态。$\blacksquare$

**引理 3（均摊线性复杂度）**：整个算法处理全部 $Q$ 次涨价的总时间复杂度为 $O(N + M + Q)$。

**证明**：
每条边 $e$ 最多被置为失效（`dead_edge[e] = true`）一次，每次失效耗时 $O(1)$；每个节点 $v$ 的 $\text{indeg}[v]$ 变成 $0$ 最多发生一次，且只在变成 $0$ 的那一刻遍历其出边列表 `dag_out[v]`。因此所有递归调用的总步数不超过 $\sum_v |\text{dag\_out}[v]| \le M$，加上初始 BFS 的 $O(N + M)$ 与 $Q$ 次查询响应，总时间严格为 $O(N + M + Q)$。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 发现边权 $\ge 1$ 且每次 $+1$ 时，保持原最短距离 $d_0(u)$ 等价于路径上所有边均未涨价（引理 1）。
- **reduce to DAG reachability** — 用一次 BFS 建出最短路 DAG，把“边涨价”转化为“删除 DAG 上的有向边”。
- **classic problem solution** — 维护每个点在最短路 DAG 上的有效入度 `indeg[v]`，入度归零时触发级联删边，以均摊 $O(N + M + Q)$ 时间在线回答所有询问（引理 2、引理 3）。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, m, q` | `int` | 站点数、边数、涨价次数 | 标准输入 | 维度 |
| `eu[i], ev[i]` | `int [mxm]` | 第 $i$ 条无向边的两个端点（0-based） | 读入减 $1$ | edge endpoints |
| `dis[u]` | `int [mxn]` | 节点 $u$ 到 $0$ 号点（原 $1$ 号站）的初始最短距离 | BFS 计算 | distance |
| `dir_to[i]` | `int [mxm]` | 若边 $i$ 在最短路 DAG 上，记录其指向的终点；否则为 `-1` | 初值 `-1` | directed target |
| `dag_out[u]` | `vector<int>[mxn]` | 最短路 DAG 中从 $u$ 出发的边编号列表 | 建 DAG 时填充 | DAG outgoing |
| `indeg[v]` | `int [mxn]` | 节点 $v$ 当前剩余的有效 DAG 入边数 | 建 DAG 时统计 | in-degree |
| `dead_edge[i]` | `bool [mxm]` | 边 $i$ 是否已在 DAG 中失效 | `false` | edge失效标记 |
| `bad[v]` | `bool [mxn]` | 节点 $v$ 是否已不满 | `false` | 节点失效标记 |
| `ans` | `int` | 当前不满的车站总数 | `0` | answer |

### 4.2 算法步骤

1. 读入 $N, M$，将端点转为 0-based（学校为 $0$ 号点），建立无向图邻接表。
2. 从 $0$ 号点运行 BFS，求出所有点的初始距离 `dis[u]`。
3. 扫描所有 $M$ 条边：若 `dis[u] + 1 == dis[v]`，则该边在 DAG 中从 $u$ 指向 $v$，令 `dir_to[i] = v`、`dag_out[u].push_back(i)`、`++indeg[v]`（对称处理 `dis[v] + 1 == dis[u]`）；其余边 `dir_to[i] = -1`。
4. 定义递归函数 `del_edge(eid)`：
   - 若 `eid < 0` 或 `dead_edge[eid]` 为真，直接返回；
   - 令 `dead_edge[eid] = true`，取出终点 `v = dir_to[eid]`；
   - 若 `v >= 0` 且 `!bad[v]`，令 `--indeg[v]`；若 `indeg[v] == 0`，则置 `bad[v] = true`、`++ans`，并对 `dag_out[v]` 中的每条出边 `nxt` 递归调用 `del_edge(nxt)`。
5. 依次读入 $Q$ 次涨价的边编号 $r$（转为 `r - 1`），调用 `del_edge(r)` 并输出 `ans`。

### 4.3 复杂度分析

- **时间复杂度**：BFS 与建图 $O(N + M)$，每条边最多失效一次、每个点最多级联展开一次，总时间复杂度为 $O(N + M + Q)$。
- **空间复杂度**：图的邻接表与边表占用 $O(N + M)$ 空间。

---

## 5. 示例代码

```cpp
//
// 题目：P1710 地铁涨价
// https://www.luogu.com.cn/problem/P1710
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005},mxm{200005};
constexpr int inf{1145141919};

int n,m,q;
int eu[mxm],ev[mxm];
int dir_to[mxm];
bool dead_edge[mxm];
vector<int>graph[mxn];
vector<int>dag_out[mxn];
int dis[mxn],indeg[mxn];
bool bad[mxn];
int ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>q;
	for(int i=0;i<m;++i)
	{
		cin>>eu[i]>>ev[i];
		--eu[i];--ev[i];
		graph[eu[i]].push_back(ev[i]);
		graph[ev[i]].push_back(eu[i]);
	}

	fill_n(dis,n,inf);
	queue<int>Q;
	dis[0]=0;
	Q.push(0);
	while(!Q.empty())
	{
		int u=Q.front();Q.pop();
		for(int v:graph[u])
			if(dis[v]==inf)
			{
				dis[v]=dis[u]+1;
				Q.push(v);
			}
	}

	fill_n(dir_to,m,-1);
	for(int i=0;i<m;++i)
	{
		int u=eu[i],v=ev[i];
		if(dis[u]+1==dis[v])
		{
			dir_to[i]=v;
			dag_out[u].push_back(i);
			++indeg[v];
		}
		else if(dis[v]+1==dis[u])
		{
			dir_to[i]=u;
			dag_out[v].push_back(i);
			++indeg[u];
		}
	}

	auto del_edge=[&](int eid,auto&self)->void{
		if(eid<0||dead_edge[eid])
			return;
		dead_edge[eid]=true;
		int v=dir_to[eid];
		if(v<0||bad[v])
			return;
		if(--indeg[v]==0)
		{
			bad[v]=true;
			++ans;
			for(int nxt:dag_out[v])
				self(nxt,self);
		}
	};

	for(int i=0,r;i<q;++i)
	{
		cin>>r;--r;
		del_edge(r,del_edge);
		cout<<ans<<'\n';
	}
	return 0;
}
```
