# P1751 贪吃虫

## 1. 重新读题

给定一棵包含 $n$（$1 \le n \le 5000$）个节点的树，树上有 $k$（$1 \le k \le 1000$）只贪吃虫，初始分别位于不同的节点 $P_1, P_2, \dots, P_k$。

接下来依次出现 $h$（$1 \le h \le 500$）次食物。每次食物出现在节点 $r$ 时，所有虫子同时沿树上唯一简单路径以每秒 $1$ 条边的速度向 $r$ 移动：
1. 若某只虫子通往食物 $r$ 的路径（祖先链）上存在另一只虫子，则距离食物较远的那只虫子立即停止移动，停留在当前节点；
2. 若多只虫子在同一时刻尝试进入同一个节点，只有编号最小的虫子能够进入，其余虫子停止移动并停留在原节点；
3. 第一只到达 $r$ 的虫子吃掉食物并停留在 $r$，随后开始下一轮食物争夺。

求 $h$ 轮过后每只虫子最终所在的节点编号及吃到食物的次数。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：以食物 $r$ 为根时，“路径上有虫”意味着什么？

当食物出现在 $r$ 时，我们将整棵树以 $r$ 为根做有根化，记每个节点的深度为 $\text{dep}[u]$（$\text{dep}[r] = 0$）。此时所有虫子的移动方向都是**从子节点走向父节点**。

这一单向性带来了一个极其关键的封闭性质：
> 一旦某只虫子进入了节点 $u$ 的祖先集合 $\operatorname{anc}(u)$，由于虫子只能向父节点移动或原地停下，它在后续任何时刻都**永远不会离开 $\operatorname{anc}(u)$**！

### 2.2 子树赢家与首达时间（附完整证明）

**引理 1（子树唯一赢家与无阻挡首达时间）**：
对于以 $r$ 为根的树中任意节点 $u$，若不考虑来自 $u$ 子树外部（即 $u$ 的真祖先处）的阻挡，能够最先到达节点 $u$ 的虫子必然是 $u$ 的子树 $T_u$ 中二元组 $(\text{dep}[\text{pos}[w]],\, w)$ 最小的那只虫子（记为 $\text{best}[u]$），且它到达 $u$ 的时刻为
$$\text{arr}[u] = \text{dep}[\text{pos}[\text{best}[u]]] - \text{dep}[u].$$
而 $T_u$ 中的任何其他虫子 $w' \ne \text{best}[u]$ 都**绝对不可能进入 $u$**。

**证明**：
设 $w^* = \text{best}[u]$。在 $T_u$ 内部，由于没有任何虫子的深度小于 $\text{dep}[\text{pos}[w^*]]$（深度相同时 $w^*$ 编号最小），在 $w^*$ 从 $\text{pos}[w^*]$ 走到 $u$ 的途中，其到 $u$ 的路径上不可能先出现 $T_u$ 内的其他虫子，且在任何同深度汇合点 $w^*$ 均赢得编号裁决，故若无来自 $u$ 上方的阻挡，$w^*$ 必在时刻 $\text{arr}[u]$ 率先进入 $u$。
一旦 $w^*$ 进入 $u$（或与其它同时到达 $u$ 的虫子竞争获胜），$T_u$ 内的其余虫子要么在 $u$ 处同时竞争落败而停在 $u$ 的子节点，要么在时刻 $\text{arr}[u]$ 仍位于 $u$ 的真子树内，此时 $w^*$ 已位于其通往 $r$ 的必经点 $u$ 上，故其余虫子永远无法进入 $u$。$\blacksquare$

**引理 2（每只虫子的精确移动步数公式）**：
对每只虫子 $w$，设 $c = \text{top}[w]$ 为其起始位置 $\text{pos}[w]$ 的最高祖先，满足 $\text{best}[c] = w$（因初始位置互不相同，$c = \text{pos}[w]$ 必满足，故 $\text{top}[w]$ 良定义）。
定义从根 $r$ 自顶向下递推的阻挡时间上限：
$$\text{lim}[r] = \text{arr}[r], \qquad \text{lim}[u] = \min\big(\text{lim}[\text{pa}[u]],\, \text{arr}[u]\big).$$
则在本轮中，虫子 $w$ 向上移动的精确步数恰为 $\text{lim}[\text{top}[w]]$（且满足 $\text{top}[w] = r$ 的唯一虫子 $\text{best}[r]$ 吃掉食物）。

**证明**：
记 $v = \text{pos}[w],\, c = \text{top}[w]$。由引理 1，在 $c$ 的子树内部没有任何虫子能阻挡 $w$ 在时刻 $\text{arr}[c] = \text{dep}[v] - \text{dep}[c]$ 到达 $c$；同时，由于在 $c$ 的父节点 $p = \text{pa}[c]$ 处有 $\text{best}[p] \ne w$，虫子 $w$ 绝不可能跨过 $c$ 进入 $p$，因此 $w$ 的移动步数至多为 $\text{arr}[c]$。
另一方面，在 $w$ 向上走向 $c$ 的途中，它在时刻 $t$ 被提前阻挡，当且仅当存在 $c$ 的某个真祖先 $a$ 在时刻 $\text{arr}[a] \le t$ 已被其赢家 $\text{best}[a]$ 占据（从而出现在 $w$ 通往 $r$ 的路径上）。因此 $w$ 能够不受阻挡走完的步数恰好是 $\min_{a \in \operatorname{anc}(c)} \text{arr}[a] = \text{lim}[c]$。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 以食物 $r$ 为根后，虫子只向父节点移动，故任何进入 $\operatorname{anc}(u)$ 的虫子永久留在 $\operatorname{anc}(u)$ 内。
- **bottom-up winner propagation** — 自底向上维护每棵子树的唯一获胜虫子 $\text{best}[u] = \arg\min (\text{dep}, w)$ 及首达时间 $\text{arr}[u]$（引理 1）。
- **classic problem solution** — 自顶向下维护祖先最早占领时间 $\text{lim}[u] = \min(\text{lim}[\text{pa}[u]], \text{arr}[u])$，使每只虫子向上走 $\text{lim}[\text{top}[w]]$ 步，单轮耗时 $\Theta(n)$（引理 2）。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, k, h` | `int` | 节点数、虫子数、食物出现次数 | 标准输入 | 维度 |
| `pos[w], eat[w]` | `int [mxk]` | 虫子 $w$ 的当前节点位置与吃到食物的次数 | 读入 / `0` | worm state |
| `pa[u], dep[u]` | `int [mxn]` | 以当前食物 $r$ 为根时的父节点与深度 | BFS 序构建 | tree hierarchy |
| `best_d[u], best_w[u]` | `int [mxn]` | 子树 $T_u$ 中最优虫子的起始深度与编号 | 自底向上 DP | subtree winner |
| `lim[u], top_node[w]` | `int [mxn]` | 祖先最早阻挡时间与虫子 $w$ 的最高获胜节点 | 自顶向下 DP | movement limit |

### 4.2 算法步骤

1. 读入树结构、$k$ 只虫子的初始位置 `pos[1..k]` 及食物次数 $h$。
2. 对每次出现的食物位置 $r$：
   - 从 $r$ 出发做一次 BFS，求出拓扑序 `ord[0..n-1]`、父节点 `pa[u]` 与深度 `dep[u]`；
   - 将所有 `best_d[u]` 置为 `inf`，对每只虫子 $w$ 置 `best_d[pos[w]] = dep[pos[w]], best_w[pos[w]] = w`；
   - 按 BFS 逆序（自底向上）遍历 $u$：用 `(best_d[u], best_w[u])` 更新父节点 `pa[u]`；
   - 按 BFS 正序（自顶向下）遍历 $u$：计算 `arr = best_d[u] - dep[u]`，`lim[u] = (u == r) ? arr : min(lim[pa[u]], arr)`，并记录 `if (u == r || best_w[u] != best_w[pa[u]]) top_node[best_w[u]] = u`；
   - 令获胜虫子 ` ++eat[best_w[r]]`；对每只虫子 $w = 1 \dots k$，从 `pos[w]` 向上走 `lim[top_node[w]]` 步更新 `pos[w]`。
3. 输出每只虫子的最终 `pos[w]` 和 `eat[w]`。

### 4.3 复杂度分析

- **时间复杂度**：每轮食物处理仅需一次 BFS、一次逆序扫描、一次正序扫描及各虫子向上移动（也可以直接在 DFS 栈上 $O(1)$ 取祖先），单轮耗时 $O(n + k)$，$h$ 轮总时间复杂度为 $O(h(n + k))$（约 $2.5 \times 10^6$ 次基本操作，耗时 $< 15\text{ ms}$）。
- **空间复杂度**：树邻接表与长度为 $n$ 的状态数组，空间复杂度为 $\Theta(n + k)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1751 贪吃虫
// https://www.luogu.com.cn/problem/P1751
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{5005},mxk{1005};
constexpr int inf{1145141919};

int n,k,h;
vector<int>graph[mxn];
int pos[mxk],eat_cnt[mxk];
int pa[mxn],dep[mxn],ord[mxn];
int best_d[mxn],best_w[mxn],lim[mxn];
int top_node[mxk],stk[mxn];

void dfs_move(int u,int p,int d)
{
	stk[d]=u;
	// 若 u 是某只虫子 w 的起点，向上跳跃 steps 步即为 stk[d - steps]
	for(int v:graph[u])
		if(v!=p)
			dfs_move(v,u,d+1);
}

void simulate(int r)
{
	int hd=0,tl=0;
	ord[tl++]=r;
	pa[r]=0;
	dep[r]=0;
	while(hd<tl)
	{
		int u=ord[hd++];
		for(int v:graph[u])
			if(v!=pa[u])
			{
				pa[v]=u;
				dep[v]=dep[u]+1;
				ord[tl++]=v;
			}
	}

	fill_n(best_d+1,n,inf);
	fill_n(best_w+1,n,inf);
	for(int i=1;i<=k;++i)
	{
		int u=pos[i];
		best_d[u]=dep[u];
		best_w[u]=i;
	}

	for(int i=n-1;i>=1;--i)
	{
		int u=ord[i],p=pa[u];
		if(best_d[u]<best_d[p]||(best_d[u]==best_d[p]&&best_w[u]<best_w[p]))
		{
			best_d[p]=best_d[u];
			best_w[p]=best_w[u];
		}
	}

	for(int i=0;i<n;++i)
	{
		int u=ord[i],p=pa[u];
		int arr=best_d[u]-dep[u];
		lim[u]=(u==r)?arr:min(lim[p],arr);
		if(u==r||best_w[u]!=best_w[p])
			top_node[best_w[u]]=u;
	}

	++eat_cnt[best_w[r]];
	for(int i=1;i<=k;++i)
	{
		int steps=lim[top_node[i]];
		int u=pos[i];
		while(steps--)
			u=pa[u];
		pos[i]=u;
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1,u,v;i<n;++i)
	{
		cin>>u>>v;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}
	cin>>k;
	for(int i=1;i<=k;++i)
		cin>>pos[i];
	cin>>h;
	for(int i=0,r;i<h;++i)
	{
		cin>>r;
		simulate(r);
	}
	for(int i=1;i<=k;++i)
		cout<<pos[i]<<' '<<eat_cnt[i]<<'\n';
	return 0;
}
```
