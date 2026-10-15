# P1262 间谍网络 题解

## 1. 重述题目

有 $n\le 3000$ 个间谍，编号 $1..n$。给一个有向图：边 $(A,B)$ 表示"A 掌握 B 的犯罪证据"。一旦我们控制（逮捕）了 A，那么 B 也自动被控制——即"沿边向外传播控制力"。

另外 $p$ 个间谍可以被收买（$1\le p\le n$），每人有一个收买金额 $\le 20000$。**只能**通过直接收买这些人作为"控制力的起点"。

目标：求控制**所有**间谍所需的最少总收买金额。如果不可能，输出无法被控制的所有间谍里编号最小的那个。

输出格式：
- 能控制：第一行 `YES`，第二行最小总费用。
- 不能：第一行 `NO`，第二行那个间谍的编号（**原始 1-based 编号，最小者**）。

---

## 2. 思考过程

### 2.1 探索机制——"沿边单向传播"

边 $(A,B)$ 的方向决定了信息流向：控制 $A$ 能顺带控制 $B$，但反过来不行。这是**单向传播**。

立刻想到一个反例来检验直觉：
- 1 控制 2，2 控制 3，3 控制 1（环）。收买 1 → 控制 1,2,3。收买 2 → 控制 2,3,1。收买 3 → 控制 3,1,2。**环里任何一人被控，其余自动被控。**
- 但如果只有 1→2，没有回边。收买 2 不能控制 1。**单向边不可逆。**

结论：环里的间谍是"铁板一块"，要么整组控制、要么一个都控制不到。**环应当被压缩成一个点。** 这是强连通分量（SCC）的经典信号。

### 2.2 简化版——把环压成点

把每个 SCC 压成一个超级节点。原图变成**有向无环图（DAG）**。在 DAG 上重新审视问题：

- 一个超级节点（即一个 SCC）内所有间谍互相能控制到。
- 边 $u\to v$ 在 DAG 上表示：控制 $u$ 中的所有间谍 → 自动控制 $v$ 中所有间谍。

**关键观察**：在 DAG 里，一个点 $v$ 能被控制，当且仅当至少有一个能"传递控制力到 $v$"的起点。控制力传播是沿 DAG 有向边向前的。

于是问题变成：**在 DAG 上选一些起点（每个起点必须落在一个"可被收买的 SCC"内），使得所有节点都被覆盖，且总收买费用最小。**

### 2.3 经典技术——SCC 分解

用 **Tarjan 算法**（在线性时间 $O(n+r)$ 内）求出所有 SCC。每个节点 $i$ 得到 $belong[i]$（所属 SCC 编号），每个 SCC 得到：
- $cmin[SCC]$：该 SCC 内所有可被收买间谍的最小费用；如果该 SCC 没有任何可被收买间谍，则 $cmin[SCC]=\infty$。
- $cnt[SCC]$：该 SCC 的大小（成员数）。

DAG 的边：对原图的每条边 $(u,v)$，若 $belong[u]\ne belong[v]$，则 DAG 有边 $belong[u]\to belong[v]$。**去重**（避免重复边干扰 indeg）。

### 2.4 进一步观察——"入度为 0"的特殊性

DAG 拓扑序中，**入度为 0 的节点**是"无源可传"的：没有任何别的 SCC 能给它传控制力。

**命题**：在 DAG 上要让所有节点被覆盖，**必须**给每一个入度为 0 的 SCC 提供控制起点。

*证明*：
- *必要性*：设 $s$ 是入度为 0 的 SCC。若我们不给 $s$ 提供任何收买起点，那么 $s$ 内任何间谍都不会被控制（因为没有任何其他 SCC 能传控制力到 $s$）。与"所有间谍都被控制"矛盾。所以必须给 $s$ 提供至少一个收买点。$\square$
- *充分性*：若我们对每个入度为 0 的 SCC 都提供了控制点，那么沿 DAG 的有向边传播，能覆盖到所有有"可达路径来自某入度 0 节点"的 SCC。DAG 中**任何**节点都至少能从一个入度 0 节点到达（沿入度边反向追溯，由于 DAG 无环，这条链必以某个入度 0 节点结束）。所以全部 SCC 都被覆盖。$\square$

**推论**：最优策略 = 对每个入度为 0 的 SCC，选它内部**最便宜**的可收买间谍；总费用 = 这些最便宜费用之和。如果任何一个入度为 0 的 SCC **没有任何**可收买间谍，则不可能控制所有间谍。

### 2.5 证明"最优性"

*命题*：上述贪心（每个入度 0 的 SCC 选内部最便宜的可收买间谍）给出**最小**总费用。

*证明*：
- *可行性*：由 2.4 的命题"充分性"，这样选能覆盖所有 SCC。
- *最优性*：设 $S$ 是入度为 0 的 SCC 集合。任何可行方案 $\mathcal{A}$ 必须在每个 $s\in S$ 内部**至少选一个**可收买间谍（否则 $s$ 无法被控制）。设 $\mathcal{A}$ 在 $s$ 内的选择总费用为 $cost_{\mathcal{A}}(s)$，则 $cost_{\mathcal{A}}(s)\ge cmin[s]$（因为 $cmin[s]$ 是 $s$ 内最小可收买费用，$cost_{\mathcal{A}}(s)$ 是至少一个可收买间谍的费用）。所以 $\sum_{s\in S} cost_{\mathcal{A}}(s)\ge\sum_{s\in S} cmin[s]$，即任何可行方案的费用都不小于贪心的费用。贪心达到下界，故最优。$\square$

**一个微妙的点**：如果同一个入度 0 SCC 内有多个可收买间谍，我们只需选**一个**（最便宜的），因为控制一个就能控制该 SCC 全部成员。

### 2.6 不可行情形——找最小不可控间谍

若存在某个入度为 0 的 SCC 没有任何可收买间谍，那么该 SCC 内所有间谍都不可控。我们要输出**原始编号最小**的那个间谍。

**算法**：在判定"不可行"时，遍历所有入度为 0 的 SCC，对每个无收买者的 SCC，找其内部所有成员的**原始编号最小值**，所有这样的最小值再取最小，即为答案。

实现：维护一个 $cmin[SCC]$ 数组（哨兵 $\infty$ 表示不可收买），另维护 $membermin[SCC]$（该 SCC 内最小原始编号）。

### 2.7 复杂度

- Tarjan：$O(n+r)$。
- DAG 构建（去重）：$O(r)$（用集合去重）。
- 遍历 SCC 与统计：$O(n+r)$。
- 总复杂度 $O(n+r)$，对 $n\le 3000$，$r\le 8000$ 绰绰有余。

### 2.8 边界与陷阱

- **0-based 与 1-based 转换**：内部 0-based，输出间谍编号时 `+1`。
- **DAG 边去重**：原图可能重复给同一对 SCC 加边；不去重会让入度计数虚高（虽然入度 ≥1 的判定不受影响，但更干净的去重避免隐患）。
- **全局数组初始化为 0 的陷阱**：`cmin` 必须显式 `fill` 为 `INF`（114514 家族哨兵），否则 0 会被误认为"可收买且费用为 0"。
- **不可行时的最小间谍编号**：必须在**所有**入度 0 且无收买者的 SCC 中取**全局最小**间谍编号，不能只取第一个找到的。
- **p 个可收买间谍**：题目保证 $1\le p\le n$，且**每个间谍最多被列出一次**（"接下来的 p 行，每行有两个整数"——编号应唯一，不重复；若重复，按更便宜的金额记录即可）。

---

## 3. 模式小结

- **探索机制** — 沿边单向传播控制力；环内是铁板一块，单向边不可逆。
- **简化版** — 把环（SCC）压成点，原图变 DAG；问题退化为"DAG 上选最小起点覆盖全部"。
- **经典技术** — Tarjan 线性时间 SCC 分解，得到 $belong/cmin$。
- **进一步观察** — 入度为 0 的 SCC 必须自己提供起点；给每个入度 0 的 SCC 选内部最便宜的可收买间谍 = 最优。
- **可行性判定** — 任何入度 0 的 SCC 若无收买者，则不可行；输出其内最小原始编号间谍。
- **复杂度** — $O(n+r)$，远小于约束。

---

## 4. 真实方案

### 4.1 符号表

| 符号 | 类型 | 含义 / 初始化 |
|------|------|-------------|
| `n` | `int` | 间谍总数（读入），$n\le 3000$ |
| `p` | `int` | 可收买间谍数（读入），$1\le p\le n$ |
| `r` | `int` | 边数（读入），$1\le r\le 8000$ |
| `mxn` | `constexpr int` | 数组容量 $= 3005$（约束 + 5 松弛） |
| `mxr` | `constexpr int` | 边数组容量 $= 8005$ |
| `INF` | `constexpr int` | 哨兵 `1145141919`（"不可收买"） |
| `cost[mxn]` | `int[]` | 间谍 $i$ 的收买金额；默认 0，读入 $p$ 个后写入 |
| `head[mxn]` | `int[]` | 链式前向星：间谍 $i$ 的出边链表头，初始化 $-1$ |
| `to[mxr]`, `nx[mxr]` | `int[]` | 前向星的边终点 / 下一条边指针 |
| `dfn[mxn]` | `int[]` | Tarjan DFS 编号（0 表示未访问） |
| `low[mxn]` | `int[]` | Tarjan low-link 值 |
| `stk[mxn]` | `int[]` | Tarjan 栈 |
| `belong[mxn]` | `int[]` | 间谍 $i$ 所属 SCC 编号；0 表示未分配 |
| `cmin[mxn]` | `int[]` | SCC $s$ 内最小可收买金额；**必须** `fill` 为 `INF` |
| `membermin[mxn]` | `int[]` | SCC $s$ 内最小原始编号（0-based）；**必须** `fill` 为 $n$（或更大） |
| `indeg[mxn]` | `int[]` | DAG 上 SCC $s$ 的入度 |
| `scc_cnt` | `int` | SCC 总数 |
| `timer` | `int` | Tarjan 全局时钟 |
| `ans` | `int` | YES 路径的总费用累加器，初始 0 |
| `bad` | `int` | NO 路径的最小不可控间谍 1-based 编号，初始 `INT_MAX` |
| `ok` | `bool` | 是否可行 |

### 4.2 算法步骤

1. **读入**：读 $n$；读 $p$ 与 $p$ 个 `(间谍, 金额)` 到 `cost[]`；读 $r$ 与 $r$ 条边 `(A,B)`（**0-based**），用链式前向星存入 `to/nx/head`。
2. **Tarjan SCC**：对每个未访问节点跑 `dfs(i)`，按 SCC 出栈顺序分配 `belong[i]`；`scc_cnt` 累加。
3. **统计 SCC 属性**：遍历 $i=0..n-1$：
   - `s = belong[i]`
   - `cmin[s] = min(cmin[s], cost[i])`（若 $i$ 可收买）
   - `membermin[s] = min(membermin[s], i)`
4. **构建 DAG 入度**：遍历每条原边 $(u,v)$，若 $belong[u]\ne belong[v]$，则 `indeg[belong[v]]++`（去重：用 `pair<int,int>` 的 `set`，或按 `(belong[u],belong[v])` 排序后去重）。
5. **可行性判定 + 费用计算**：遍历 $s=0..scc_cnt-1$，仅看 `indeg[s]==0` 的 SCC：
   - 若 `cmin[s]==INF`：置 `ok=false`，记录 `bad = min(bad, membermin[s]+1)`（1-based）。
   - 否则：累加 `ans += cmin[s]`。
6. **输出**：
   - `ok==true`：`YES\n` + `ans\n`
   - `ok==false`：`NO\n` + `bad\n`（`bad` 是所有入度 0 且无收买者 SCC 中，最小间谍 1-based 编号的全局最小）

### 4.3 复杂度

- 时间：$O(n+r + \text{SCC} + \text{sort}) = O((n+r)\log(n+r))$（含去重排序；若用 set 则 $O((n+r)\log(n+r))$）。
- 空间：$O(n+r)$。

### 4.4 验证状态

本方案基于经典 Tarjan SCC + DAG 拓扑分析的教科书解法。正确性由 §2.4 的必要性 + 充分性命题与 §2.5 的最优性命题共同保证。边界情况：
- $n=1$，$p=1$，$r=1$（边为 $(1,1)$ 自环）：单点 SCC，入度 0（自环不产生跨 SCC 边），可收买 → YES + 该金额。
- 入度 0 的 SCC 无收买者：NO + 其内最小间谍编号。
- 多个入度 0 的 SCC 都无收买者：取全局最小间谍编号。

---

## 5. 示例代码（C++）

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{3005};
constexpr int mxr{8005};
constexpr int INF{1145141919};

int n,p,r;
int cost[mxn];                       // 0 = 不可收买（读入时区分）
int head[mxn],to[mxr],nx[mxr],ecnt=0;

// Tarjan 状态
int dfn[mxn],low[mxn],stk[mxn],top=0,timer=0;
bool instack[mxn]{};
int belong[mxn],scc_cnt=0;
int cmin[mxn],membermin[mxn],indeg[mxn]{};   // indeg 必须显式清零（全局数组默认 0 是巧合，写清楚更安全）

void addedge(int u,int v)
{
	to[ecnt]=v;nx[ecnt]=head[u];head[u]=ecnt++;
}

void dfs(int u)
{
	dfn[u]=low[u]=++timer;
	stk[top++]=u;instack[u]=true;
	for(int e=head[u];e!=-1;e=nx[e])
	{
		int v=to[e];
		if(!dfn[v])
		{
			dfs(v);
			low[u]=min(low[u],low[v]);
		}
		else if(instack[v])
			low[u]=min(low[u],dfn[v]);
	}
	if(low[u]==dfn[u])
	{
		while(true)
		{
			int w=stk[--top];
			instack[w]=false;
			belong[w]=scc_cnt;
			if(w==u)
				break;
		}
		scc_cnt++;
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	// 读入可收买
	// cost 默认 0，读入后非 0 表示可收买
	cin>>p;
	for(int i=0,sp,fee;i<p;i++)
	{
		cin>>sp>>fee;--sp;
		// 同一间谍可能出现多次（题目未禁止），取最小
		if(cost[sp]==0)
			cost[sp]=fee;
		else
			cost[sp]=min(cost[sp],fee);
	}

	// 读入边
	fill_n(head,n,-1);
	cin>>r;
	for(int i=0,a,b;i<r;i++)
	{
		cin>>a>>b;--a;--b;
		addedge(a,b);
	}

	// Tarjan
	for(int i=0;i<n;i++)
		if(!dfn[i])
			dfs(i);

	// 统计 SCC 属性：cmin = INF 表示不可收买；membermin 初始化为大值
	fill_n(cmin,scc_cnt,INF);
	for(int i=0;i<scc_cnt;i++)
		membermin[i]=n;            // 比最大 0-based 编号 (n-1) 还大
	for(int i=0;i<n;i++)
	{
		int s=belong[i];
		if(cost[i]!=0)
			cmin[s]=min(cmin[s],cost[i]);
		membermin[s]=min(membermin[s],i);
	}

	// DAG 入度（去重）
	set<pair<int,int>>es;
	for(int u=0;u<n;u++)
		for(int e=head[u];e!=-1;e=nx[e])
		{
			int v=to[e];
			if(belong[u]!=belong[v])
				es.insert({belong[u],belong[v]});
		}
	for(auto [u,v]:es)
		indeg[v]++;

	// 可行性 + 最小费用
	int ans=0;                  // YES 路径累加费用
	int bad=INT_MAX;            // NO 路径记录最小不可控间谍（1-based）
	bool ok=true;
	for(int s=0;s<scc_cnt;s++)
	{
		if(indeg[s]==0)
		{
			if(cmin[s]==INF)
			{
				ok=false;
				bad=min(bad,membermin[s]+1);   // 1-based
			}
			else
				ans+=cmin[s];
		}
	}

	if(ok)
	{
		cout<<"YES\n"<<ans<<'\n';
	}
	else
	{
		cout<<"NO\n"<<bad<<'\n';
	}
	return 0;
}
```

### 代码说明

- **链式前向星**（`head/to/nx`）：经典稀疏图存储，$O(1)$ 加边，$O(\deg)$ 遍历出边。
- **Tarjan**：标准 DFS + 栈实现，`dfn/low/instack/belong` 四件套。
- **去重**：用 `set<pair<int,int>>` 收集 DAG 边，避免重复入度计数。
- **不可行时的最小间谍编号**：`bad` 初始 `INT_MAX`，遍历所有入度 0 且无收买者的 SCC 时取 `min(bad, membermin[s]+1)`（1-based）。
- **`cost[i]==0` 语义**：题目保证收买金额 $\ge 1$（"愿意收受的具体数额"），故 0 表示"不可收买"。若题目允许 0 金额，需要改用单独的 `bought[]` 标志。

### 复杂度报告

- 时间 $O((n+r)\log(n+r))$（主要开销在 set 去重；Tarjan 本身 $O(n+r)$）。
- 空间 $O(n+r)$。
- $n=3000$，$r=8000$ 时，set 去重约 8000 次插入，远低于 1s 时限。
