# P1726 上白泽慧音

## 1. 重新读题

给定一张包含 $N$ 个顶点（$1 \le N \le 5000$）和 $M$ 条边（$0 \le M \le 5 \times 10^4$）的有向图（其中 $t = 1$ 为单向边 $a \to b$，$t = 2$ 为双向边 $a \leftrightarrow b$）。

定义“绝对连通区域”为极大的顶点子集 $S$，使得 $S$ 中任意两点均可互相到达（即有向图的**强连通分量 SCC**）。要求找出包含顶点数最多的强连通分量（若大小并列，选择内部顶点升序排列后字典序最小的那一个），输出其大小及升序排列的顶点编号。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：强连通分量与字典序平局裁决

题目中定义的“绝对连通区域”就是图论标准概念——极大强连通分量（Strongly Connected Component, SCC）。由于有向图的所有强连通分量构成顶点集 $V$ 的一个**不交划分**（关系 $\langle A, B \rangle$ 是等价关系），任何两个不同的强连通分量没有公共顶点！

这意味着：当两个强连通分量大小同为最大值时，比较它们的升序顶点序列的字典序，等价于**直接比较它们各自包含的最小顶点编号**（最小编号更小者字典序必然更小，且不可能相等，因为两分量不相交）！

### 2.2 Tarjan 强连通分量算法（附完整证明）

**引理 1（Tarjan DFS 树与 `low` 值割根定理）**：对有向图运行 DFS，维护每个节点的访问时间戳 `dfn[u]`，以及搜索栈 `stk`（栈中保留当前已访问但尚未归入已闭合 SCC 的节点）。定义 `low[u]` 为从 $u$ 的 DFS 子树出发，经过至多一条非树边（返祖边或横叉边）能到达的仍在栈中的最小 `dfn` 值。则节点 $u$ 是其所在强连通分量在 DFS 树上的最高祖先（分量之根），当且仅当回溯完 $u$ 的所有出边后有 `dfn[u] == low[u]`。

**证明**：
- **必要性**：若 $u$ 是某个强连通分量 $C$ 中最早被访问的节点，则 $C$ 中所有其他节点都在 $u$ 的 DFS 子树内（`dfn[v] > dfn[u]`），且从 $C$ 出发的任何边都不可能通向仍在栈中且 `dfn < dfn[u]` 的祖先 $w$（否则 $w$ 能通过树边到 $u$、$u$ 又能通过该路径回到 $w$，导致 $w \in C$，与 $u$ 最早被访问矛盾）。故必有 `low[u] == dfn[u]`。
- **充分性**：若 `low[u] == dfn[u]`，说明 $u$ 的子树中残留栈内的节点无法到达任何 $u$ 的真祖先，故 $u$ 无法与 $u$ 之上的任何节点强连通；而此时栈中位于 $u$ 上方的所有节点均尚未在更深的根处满足 `low == dfn`，故它们都能到达自己的祖先直至 $u$，与 $u$ 互相可达，从而弹出栈中直至 $u$ 的全部节点恰好构成一个完整的极大强连通分量。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 将“绝对连通区域”识别为有向图的极大强连通分量（SCC），并利用 SCC 互不相交的性质简化平局时的字典序比较。
- **classic problem solution** — 使用 Tarjan 算法在 $O(N + M)$ 时间内求出所有 SCC，对每个 SCC 内部顶点升序排序后取 `(size, -first_elem)` 最优者（引理 1）。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, m` | `int` | 顶点数与边数 | 标准输入 | 维度 |
| `graph[u]` | `vector<int>[mxn]` | 有向图邻接表 | 读入构建 | adjacency list |
| `dfn[u], low[u]` | `int [mxn]` | Tarjan 时间戳与追溯值 | `0` | Tarjan arrays |
| `stk, instk` | `int[], bool[]` | Tarjan 搜索栈与在栈标记 | 空 | stack |
| `best` | `vector<int>` | 当前最优强连通分量的有序顶点列表 | 空 | best SCC |

### 4.2 算法步骤

1. 读入 $N, M$ 及各边 $(a, b, t)$：添加有向边 $a \to b$；若 $t = 2$ 再添加 $b \to a$。
2. 对所有未访问顶点 $i = 1 \dots N$ 调用 `tarjan(i)`：
   - 分配 `dfn[u] = low[u] = ++timer`，将 $u$ 压栈并置 `instk[u] = true`；
   - 遍历出边 $v$：若未访问则递归调用并用 `low[v]` 更新 `low[u]`；若已在栈中则用 `dfn[v]` 更新 `low[u]`；
   - 若回溯后 `dfn[u] == low[u]`，弹出栈顶直至 $u$ 组成集合 `cur`，将 `cur` 升序排序；若 `cur.size() > best.size()` 或（大小相等且 `cur[0] < best[0]`），令 `best = move(cur)`。
3. 输出 `best.size()` 及 `best` 中的所有顶点编号。

### 4.3 复杂度分析

- **时间复杂度**：Tarjan 遍历每个点和每条边常数次，各不相交分量内部排序总耗时 $\sum |C_i| \log |C_i| \le O(N \log N)$，总时间复杂度为 $O(N \log N + M)$。
- **空间复杂度**：邻接表与辅助栈占用 $O(N + M)$ 空间。

---

## 5. 示例代码

```cpp
//
// 题目：P1726 上白泽慧音
// https://www.luogu.com.cn/problem/P1726
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{5005};

int n,m;
vector<int>graph[mxn];
int dfn[mxn],low[mxn],timer;
int stk[mxn],top;
bool instk[mxn];
vector<int>best;

void tarjan(int u)
{
	dfn[u]=low[u]=++timer;
	stk[top++]=u;
	instk[u]=true;
	for(int v:graph[u])
	{
		if(!dfn[v])
		{
			tarjan(v);
			low[u]=min(low[u],low[v]);
		}
		else if(instk[v])
			low[u]=min(low[u],dfn[v]);
	}
	if(dfn[u]==low[u])
	{
		vector<int>cur;
		while(true)
		{
			int x=stk[--top];
			instk[x]=false;
			cur.push_back(x);
			if(x==u)
				break;
		}
		sort(cur.begin(),cur.end());
		if(cur.size()>best.size()||(cur.size()==best.size()&&cur[0]<best[0]))
			best=move(cur);
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0,u,v,t;i<m;++i)
	{
		cin>>u>>v>>t;
		graph[u].push_back(v);
		if(t==2)
			graph[v].push_back(u);
	}
	for(int i=1;i<=n;++i)
		if(!dfn[i])
			tarjan(i);

	cout<<best.size()<<'\n';
	for(size_t i=0;i<best.size();++i)
		cout<<best[i]<<(i+1==best.size()?'\n':' ');
	return 0;
}
```
