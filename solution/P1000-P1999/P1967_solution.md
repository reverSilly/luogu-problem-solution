# P1967 [NOIP 2013 提高组] 货车运输

## 1. 重新读题

### 1.1 问题重述（人话版）

有 $n$ 座城市、$m$ 条**双向**道路，第 $i$ 条连接 $x_i,y_i$，限重 $z_i$。一辆货车从 $x$ 到 $y$，走一条路径，能运的货物重量不能超过路径上**所有道路限重的最小值**（路径的「瓶颈」）。问每辆货车能运的最大重量；不连通输出 $-1$。

形式化：对每个询问 $(u,v)$，求

$$
\beta(u,v)=\max_{P:u\to v}\ \min_{e\in P} z_e ,
$$

若 $u,v$ 不连通则为 $-1$。

### 1.2 关键约束与规模暗示

- $n<10^4$，$m<5\times10^4$，$q<3\times10^4$，$0\le z\le 10^5$。
- $30\%$：$n,m,q<10^3$，每次询问做一遍「最大化最小值」的 Dijkstra/BFS（类似最大瓶颈路）即可，$O(qm\log n)$ 勉强。
- $100\%$：$q=3\times10^4$，$m=5\times10^4$，逐询问最短路 $3\times10^4\times5\times10^4=1.5\times10^9$ 过不了。需要**一次预处理、$O(\log n)$ 回答**。
- $z$ 可以等于 $0$，所以不能用「$0$ 表示不连通」的哨兵；不连通必须单独判断。
- 可能有重边、自环不会出现（$x\ne y$），多座城市可能分成多个连通块。

---

## 2. 思维链（Chain-of-thought）

### 2.1 先看「最大瓶颈路」有什么结构

题目关心的不是路径长度之和，而是路径上**最小的那条边**。直觉：细边会卡住整条路径，所以应该尽量走「宽」的边。

**观察 1**：如果一条边 $e$ 的限重很小，它几乎不会被任何最优路径使用。更准确地说，我们希望路径经过的边权的最小值尽可能大。

**观察 2**：把边按限重从大到小依次加入，当 $u$ 和 $v$ **第一次连通**时，那条刚加入的边权就是 $\beta(u,v)$。因为在此之前，只使用 $\ge$ 该权重的边无法连通；而加入后，路径上的边都 $\ge$ 该权重。

这个「从大到小加边、并查集判连通」的过程正是**最大生成树（Kruskal）**。于是猜想：

> 任意一棵最大生成树上 $u\to v$ 路径的最小边权，就等于 $\beta(u,v)$。

### 2.2 证明「最大生成树保留瓶颈」

先明确最大生成树（森林）的定义：在连通块内边权和最大的生成树；不连通时对每个连通块各取一棵。

**引理 1（最大生成树的割性质）**：设 $T$ 是某连通块的一棵最大生成树。对 $T$ 中任意一条边 $e$，删去 $e$ 把点集分成 $S$ 与 $V\setminus S$ 两部分（这是一个割）。则对任意一条横跨该割的边 $f$，都有 $z_f\le z_e$。

**证明**：反设存在横跨割的边 $f$ 满足 $z_f>z_e$。因为 $f$ 的两个端点分居割的两侧，$T-e+f$ 仍是一棵生成树（用 $f$ 把两棵子树接起来），其边权和为

$$
\sum_{e'\in T}z_{e'}-z_e+z_f>\sum_{e'\in T}z_{e'},
$$

与 $T$ 边权和最大矛盾。$\square$

> 这条性质其实就是最大生成树的「环性质 / 割性质」，是 Kruskal 正确性的核心。

**引理 2（最大生成树路径最小边 $=\beta(u,v)$）**：设 $u,v$ 在同一连通块，$P_T$ 是 $T$ 上 $u\to v$ 的唯一路径，令

$$
w^*=\min_{e\in P_T}z_e .
$$

则 $\beta(u,v)=w^*$。

**证明**：

- **$\beta\ge w^*$**：$P_T$ 本身就是一条 $u\to v$ 路径，它的最小边权是 $w^*$，所以所有路径中的最大值不小于它。
- **$\beta\le w^*$**：取 $P_T$ 上使 $z_e=w^*$ 的边 $e$，删去 $e$ 得到割 $(S,V\setminus S)$，$u,v$ 分居两侧。任取一条 $u\to v$ 的路径 $P$，它必须从 $S$ 跨到另一侧，所以 $P$ 中至少有一条横跨该割的边 $f$。由引理 1，$z_f\le z_e=w^*$。于是 $P$ 的最小边权 $\le z_f\le w^*$。对任意 $P$ 都成立，故 $\beta\le w^*$。

两边夹逼，$\beta=w^*$。$\square$

由此，**只需要建出最大生成森林，再回答树上两点路径的最小边权**。不连通直接 $-1$。

### 2.3 怎么建最大生成森林

Kruskal：把所有边按 $z$ **降序**排序；并查集维护连通块；若两端不在同一块就选入这条边，并在树上加一条无向边。

**引理 3（Kruskal 降序得到最大生成森林）**：上述过程对每个连通块得到的是一棵最大生成树。

**证明**：这是 Kruskal 的标准割性质证明。按降序考虑边 $e=(u,v)$：若 $u,v$ 已连通，$e$ 与当前森林构成环，环上已有边的权都 $\ge z_e$（因为它们更早被加入），由环性质 $e$ 不可能改进最优解；若 $u,v$ 不连通，当前割（$u$ 所在连通块与其他块）中，所有尚未考虑的边权都 $\le z_e$，而 $e$ 是当前能连接两块的最大边，由割性质它属于某棵最大生成树。归纳即得每个连通块的生成树边权和最大。$\square$

> 也可以直接引用「最大生成树 = 把边权取负后的最小生成树」，但上面的割论证更贴题。

### 2.4 树上路径最小边：倍增 LCA

建出森林后，$q$ 个询问要在 $O(\log n)$ 内回答。经典做法是倍增 LCA，同时维护「向上 $2^j$ 步经过的边的最小值」。

对每个点 $u$ 预处理：

- `gf[u][j]`：$u$ 的 $2^j$ 级祖先；
- `mn[u][j]`：从 $u$ 向上走 $2^j$ 条边（到达 `gf[u][j]`）经过的边的最小值；
- `dep[u]`：深度，根深度为 $0$。

转移：

$$
\texttt{gf}[u][j]=\texttt{gf}\big[\texttt{gf}[u][j-1]\big][j-1],\qquad
\texttt{mn}[u][j]=\min\big(\texttt{mn}[u][j-1],\texttt{mn}[\texttt{gf}[u][j-1]][j-1]\big).
$$

根节点令 `gf[root][0]=root`、`mn[root][0]=+\infty`，这样越界跳祖先不会引入错误的 $0$。

**询问流程**：

1. 若 `getrt(u)!=getrt(v)`，输出 $-1$。
2. 把深的点往上跳到与另一点同深度，沿途对 `mn` 取 min。
3. 若此时两点相同，答案就是沿途 min。
4. 否则从高位到低位同时上跳：若 `gf[u][j]!=gf[v][j]`，说明 LCA 还在上面，两边各跳 $2^j$ 并把 `mn[u][j],mn[v][j]` 纳入 min。
5. 最后再纳入两条「一跳到达 LCA」的边的 min。

**引理 4（倍增询问正确）**：上述流程返回 $u\to v$ 树上路径的最小边权。

**证明**：步骤 2 用二进制拆分把深度差走完，每段恰好被某个 `mn` 覆盖，不重不漏（二进制表示唯一）。步骤 4 的判定 `gf[u][j]!=gf[v][j]` 保证跳完后 LCA 仍在两者上方，且不会跳过；标准倍增 LCA 的正确性保证最终 $u,v$ 成为 LCA 的两个不同孩子，此时路径分解为「$u$ 到 LCA」与「$v$ 到 LCA」两段，所有边都恰好被某个 `mn` 区间覆盖，取 min 即为路径最小边。$\square$

### 2.5 失败尝试与坑点

- **直接对每个询问跑最大瓶颈 Dijkstra**：$O(qm\log n)\approx1.5\times10^9$，超时。这个思路在 $30\%$ 的小数据可行，正是子任务给的提示。
- **只用最大生成树但不用 LCA，逐询问暴力爬树**：树可能退化成链，$O(nq)=3\times10^8$ 也可能超时；倍增是稳妥的 $O((n+q)\log n)$。
- **不连通判定**：不能靠「路径 min 为 $0$」判断，因为 $z$ 允许为 $0$，合法的限重 $0$ 路径与不连通会混淆。必须用并查集判连通。
- **`mn` 的初值**：全局数组默认 $0$，如果根节点的 `mn[root][0]` 留 $0$，取 min 时会把答案错误地压成 $0$（尤其当真实答案是正数时）。必须显式填 `1145141919`。
- 边权 $z\le10^5$，答案为 `int` 即可；`INF` 用 `1145141919` 不会与合法值冲突。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 关心路径上最小边权，「细边卡脖子」。
- **reduce to easier problem** — 最大瓶颈路的值等于最大生成树上路径的最小边权。
- **classic problem solution** — Kruskal 降序建最大生成森林；割性质 + 环性质证明。
- **classic problem solution** — 倍增 LCA 同时维护向上 $2^j$ 步的边权最小值，$O(\log n)$ 回答。
- **handle connectivity** — 并查集判连通，不连通输出 $-1$，不能拿 $0$ 当哨兵。

---

## 4. 真实解法（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n,m,q` | `int` | 城市数、道路数、询问数 | 读入 | 规模 |
| `E[]` | `edge [50005]` | 边表 `{u,v,w}` | 读入 | edge |
| `tr[u]` | `vector<pair<int,int>> [10005]` | 最大生成森林邻接表 `{v,w}` | 空 | tree |
| `f[]` | `int [10005]` | 并查集父指针 | `f[i]=i` | father |
| `dep[]` | `int [10005]` | 深度 | 0 | depth |
| `gf[u][j]` | `int [10005][20]` | $u$ 的 $2^j$ 级祖先 | 根指向自己 | grand father |
| `mn[u][j]` | `int [10005][20]` | 向上 $2^j$ 步的最小边权 | 根的为 `INF` | minimum |
| `INF` | `constexpr int` | $1145141919$ | — | 无穷 |

### 4.2 算法步骤

1. 读入 $n,m$，边下标转 0-based 存入 `E`；初始化并查集。
2. 按 `w` 降序排序 `E`。依次扫描：若两端不在同一并查集，合并并把这条边加入森林（双向）。
3. 对每个并查集根 $r$（`getrt(r)==r`），以 $r$ 为根做一次迭代 DFS/BFS，设置 `dep`，并对每个节点预处理 `gf[.][0..19]` 与 `mn[.][0..19]`。
4. 读入每个询问 $u,v$：
   - 若 `getrt(u)!=getrt(v)`，输出 `-1`；
   - 否则按 2.4 的倍增流程求路径最小边并输出。

### 4.3 复杂度分析

- **时间**：排序 $O(m\log m)$；Kruskal $O(m\alpha(n))$；预处理 $O(n\log n)$；每个询问 $O(\log n)$。总计 $O(m\log m+(n+q)\log n)$。
- **空间**：$O(m+n\log n)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1967 [NOIP 2013 提高组] 货车运输
// https://www.luogu.com.cn/problem/P1967
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005},mxm{50005},mxj{20};
constexpr int inf{1145141919};

struct edge
{
	int u,v,w;
};

int n,m,q;
edge E[mxm];
vector<pair<int,int>>tr[mxn];
int f[mxn],dep[mxn],gf[mxn][mxj],mn[mxn][mxj];

int getrt(int x)
{
	if(f[x]==x)
		return x;
	return f[x]=getrt(f[x]);
}

bool cmp(edge a,edge b){return a.w>b.w;}

void dfs(int rt)
{
	dep[rt]=0;
	gf[rt][0]=rt;
	mn[rt][0]=inf;
	for(int j=1;j<mxj;++j)
	{
		gf[rt][j]=rt;
		mn[rt][j]=inf;
	}
	vector<int>st{rt};
	while(!st.empty())
	{
		int u=st.back();
		st.pop_back();
		for(auto[v,w]:tr[u])
		{
			if(v==gf[u][0])
				continue;
			dep[v]=dep[u]+1;
			gf[v][0]=u;
			mn[v][0]=w;
			for(int j=1;j<mxj;++j)
			{
				gf[v][j]=gf[gf[v][j-1]][j-1];
				mn[v][j]=min(mn[v][j-1],mn[gf[v][j-1]][j-1]);
			}
			st.push_back(v);
		}
	}
}

int ask(int u,int v)
{
	int res{inf};
	if(dep[u]<dep[v])
		swap(u,v);
	int d=dep[u]-dep[v];
	for(int j=0;j<mxj;++j)
		if(d>>j&1)
		{
			res=min(res,mn[u][j]);
			u=gf[u][j];
		}
	if(u==v)
		return res;
	for(int j=mxj-1;j>=0;--j)
		if(gf[u][j]!=gf[v][j])
		{
			res=min(res,min(mn[u][j],mn[v][j]));
			u=gf[u][j];
			v=gf[v][j];
		}
	res=min(res,min(mn[u][0],mn[v][0]));
	return res;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<m;++i)
	{
		cin>>E[i].u>>E[i].v>>E[i].w;
		--E[i].u;
		--E[i].v;
	}
	for(int i=0;i<n;++i)
		f[i]=i;
	sort(E,E+m,cmp);
	for(int i=0;i<m;++i)
	{
		int ru{getrt(E[i].u)},rv{getrt(E[i].v)};
		if(ru==rv)
			continue;
		f[ru]=rv;
		tr[E[i].u].push_back({E[i].v,E[i].w});
		tr[E[i].v].push_back({E[i].u,E[i].w});
	}
	for(int i=0;i<n;++i)
		if(getrt(i)==i)
			dfs(i);

	cin>>q;
	for(int i=0,u,v;i<q;++i)
	{
		cin>>u>>v;
		--u;
		--v;
		if(getrt(u)!=getrt(v))
			cout<<-1<<'\n';
		else
			cout<<ask(u,v)<<'\n';
	}
	return 0;
}
```
