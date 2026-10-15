# P1073 [NOIP 2009 提高组] 最优贸易 题解

---

## 1. 重新读题 — 确认问题边界

给定一张 $n$ 个点 $m$ 条边的**混合图**（既有单向边 $z=1$ 也有双向边 $z=2$），每个点 $i$ 有一个**买入价 = 卖出价**为 $a_i$。商贩从 $1$ 号点出发，最终到达 $n$ 号点，途中可以**任意重复经过节点**。他最多进行**一次**买卖：在某个经过的城市买入，在**之后**到达的某个城市卖出，赚取差价 $a_{\text{卖出}} - a_{\text{买入}}$。如果无法获利则不做交易，输出 $0$。

约束：
- $n \le 10^5$，$m \le 5 \times 10^5$，$a_i \le 100$
- 保证 $1$ 号可到达 $n$ 号
- 图中有环（双向边 + 单向边组合），50% 数据存在环

需要求：最大可能差价。

---

## 2. 思路链 — 发现、引理与证明

### 2.1 一个朴素的方向

最直接的想法：枚举在 $p$ 买入、在 $q$ 卖出，要求存在路径 $1 \leadsto p \leadsto q \leadsto n$。三层枚举 $O(n^3)$ 不可接受。

观察发现，路径可以**在某个中间点 $u$ 处切开**：

$$1 \leadsto \underbrace{u}_{\text{切开}} \leadsto n$$

- 前半段 $1 \leadsto u$：我们关心的是**最小买入价**
- 后半段 $u \leadsto n$：我们关心的是**最大卖出价**

只要能高效计算每个 $u$ 的这两个值，答案就是 $\max_u(\text{后半段最大值} - \text{前半段最小值})$。

### 2.2 形式化定义

对每个节点 $u$ 定义两个量：

$$
\begin{aligned}
\operatorname{mn}[u] &\triangleq \min\{\,a_x \mid x \text{ 在某条从 } 1 \text{ 到 } u \text{ 的路径上}\,\} \\[2mm]
\operatorname{mx}[u] &\triangleq \max\{\,a_x \mid x \text{ 在某条从 } u \text{ 到 } n \text{ 的路径上}\,\}
\end{aligned}
$$

直观含义：
- $\operatorname{mn}[u]$：从 $1$ 到 $u$ 的路上能遇到的最低价格
- $\operatorname{mx}[u]$：从 $u$ 到 $n$ 的路上能遇到的最高价格

### 2.3 引理 1 — mn 与 mx 的递推关系

**引理 1.1（正向递推）**：$\operatorname{mn}[u]$ 满足

$$
\operatorname{mn}[1] = a_1,\qquad
\operatorname{mn}[v] = \min\!\Bigl(\operatorname{mn}[v],\;\min\bigl(\operatorname{mn}[u],\,a_v\bigr)\Bigr),\quad \forall\,(u\to v)\in E
$$

*证明*：设 $P$ 是某条从 $1$ 到 $v$ 的路径。若 $P$ 仅为单点 $v=1$，则 $\operatorname{mn}[1]=a_1$ 显然。否则 $P$ 的最后一步是 $u\to v$，去掉该步得到从 $1$ 到 $u$ 的子路径 $P'$。$P$ 上所有点价格的最小值为 $\min(\min_{x\in P'} a_x,\, a_v)$。由 $\operatorname{mn}[u]$ 定义，$\min_{x\in P'} a_x \ge \operatorname{mn}[u]$，故 $\operatorname{mn}[v]\le \min(\operatorname{mn}[u],\,a_v)$。对所有前驱 $u$ 和所有路径取最小值即得递推式。$\square$

**引理 1.2（反向递推）**：$\operatorname{mx}[u]$ 满足

$$
\operatorname{mx}[n] = a_n,\qquad
\operatorname{mx}[u] = \max\!\Bigl(\operatorname{mx}[u],\;\max\bigl(\operatorname{mx}[v],\,a_u\bigr)\Bigr),\quad \forall\,(u\to v)\in E
$$

*证明*：对称于引理 1.1，在反向图上考虑。$\square$

### 2.4 引理 2 — 答案等于 $\max(\operatorname{mx}[u] - \operatorname{mn}[u])$

记 $\text{OPT}$ 为原问题的最优利润（不交易则为 $0$）。

**定理 2**：
$$
\text{OPT} = \max_{1\le u\le n}\bigl(\operatorname{mx}[u] - \operatorname{mn}[u]\bigr)
$$

*证明分上下两界*：

1. **$\text{OPT} \le \max_u(\operatorname{mx}[u]-\operatorname{mn}[u])$**：设最优策略在 $p$ 买入、在 $q$ 卖出，对应路径 $1\leadsto p\leadsto q\leadsto n$。对该路径上的任意节点 $u$：
   - 从 $1$ 到 $u$ 经过了 $p$，故 $\operatorname{mn}[u] \le a_p$
   - 从 $u$ 到 $n$ 经过了 $q$，故 $\operatorname{mx}[u] \ge a_q$
   因此 $\operatorname{mx}[u]-\operatorname{mn}[u] \ge a_q-a_p = \text{OPT}$。对所有 $u$ 取 $\max$ 得 $\max_u(\operatorname{mx}[u]-\operatorname{mn}[u]) \ge \text{OPT}$。

2. **$\text{OPT} \ge \max_u(\operatorname{mx}[u]-\operatorname{mn}[u])$**：对任意 $u$，存在一条从 $1$ 到 $u$ 的路径 $P_1$ 取到 $\operatorname{mn}[u]$（记取到该值的点为 $p$），和一条从 $u$ 到 $n$ 的路径 $P_2$ 取到 $\operatorname{mx}[u]$（记取到该值的点为 $q$）。将 $P_1$ 与 $P_2$ 在 $u$ 处拼接，得到合法路径 $1\leadsto p\leadsto u\leadsto q\leadsto n$，其中 $p$ 必然在 $q$ 之前（$p$ 在 $u$ 前，$q$ 在 $u$ 后）。故可在 $p$ 买入、$q$ 卖出，获利 $\operatorname{mx}[u]-\operatorname{mn}[u]$，于是 $\text{OPT} \ge \max_u(\operatorname{mx}[u]-\operatorname{mn}[u])$。

综合上下两界，等式成立。$\square$

### 2.5 传播算法与收敛性

引理 1 给出了递推关系，但图中有环，不能拓扑排序。然而注意到：
- $\operatorname{mn}[u]$ **只会单调递减**（每次更新只会变得更小）
- $\operatorname{mx}[u]$ **只会单调递增**（每次更新只会变得更大）
- 价格值域 $[1,100]$ 非常小

因此可以用类似 SPFA 的队列传播：

- **正向传播**：初始 $\operatorname{mn}[1]=a_1$，其余 $+\infty$。队首 $u$ 弹出后，检查每条出边 $u\to v$，若 $\operatorname{mn}[v] > \min(\operatorname{mn}[u],a_v)$ 则更新并入队。
- **反向传播**：在**反向图**上从 $n$ 出发，初始 $\operatorname{mx}[n]=a_n$，其余 $-\infty$。对反向边 $u\to v$（原图 $v\to u$），若 $\operatorname{mx}[v] < \max(\operatorname{mx}[u],a_v)$ 则更新。

**收敛性**：每个节点的 $\operatorname{mn}$ 最多减小 $a_1 - 1 \le 99$ 次，$\operatorname{mx}$ 最多增大 $100 - a_n \le 99$ 次。每次更新导致入队一次，故总入队次数 $O(100n)$，每条边在每次入队时检查一次，总复杂度 $O(100(n+m))$，对 $n=10^5,m=5\times10^5$ 完全足够。

### 2.6 另一种思路（选读）：Tarjan 缩点 + DAG DP

对于熟悉强连通分量的读者，另一种解法：
- 用 Tarjan 将图缩点为 DAG
- 每个 SCC 内部可任意往返，故取该 SCC 的 `min_price` 和 `max_price`
- 在 DAG 上拓扑 DP：
  - 正向 DP 求从源 SCC 到每个 SCC 的路径最小价格
  - 反向 DP 求从每个 SCC 到汇 SCC 的路径最大价格
- 答案 = $\max(\text{反向}_\text{SCC} - \text{正向}_\text{SCC})$

但 SPFA 传播已经足够简洁高效，且代码量更小。

---

## 3. 模式总结

| 模式 | 要点 |
|------|------|
| **分界点拆分** | 将路径在 $u$ 处切开，$1\leadsto u$ 管买入，$u\leadsto n$ 管卖出，互不干扰 |
| **双图互逆** | 用正向图求 $\operatorname{mn}$，反向图求 $\operatorname{mx}$，对称建图便于编码 |
| **单调松弛** | $\operatorname{mn}$ 只减不增、$\operatorname{mx}$ 只增不减；值域极小，松弛次数有上界 |
| **队列传播** | 类 SPFA 的 BFS 队列：更新即入队，用值域小保证复杂度而非负环判断 |

**核心观察链**：
路径可重复 → 分界点拆分 → 定义 $\operatorname{mn}/\operatorname{mx}$ → 递推关系 → 有环不可拓扑 → 值域小所以 SPFA 可收敛 → 双遍 BFS。

---

## 4. 正式解法与符号表

### 符号约定

| 符号 | 含义 |
|------|------|
| $n,m$ | 点数、边数 |
| $a_u$ | 城市 $u$ 的水晶球价格 |
| $g[u]$ | 正向邻接表 |
| $rg[u]$ | 反向邻接表（每条边反向） |
| $\operatorname{mn}[u]$ | 从 $1$ 到 $u$ 路径上的最低价格；不可达则为 $+\infty$ |
| $\operatorname{mx}[u]$ | 从 $u$ 到 $n$ 路径上的最高价格；不可达则为 $-\infty$ |
| $\text{INF}$ | $1145141919$ — 正无穷哨兵 |
| $\text{opt}$ | 最终答案 |

### 算法步骤

```
1. 读入 n,m, 价格数组 a[]
2. 建图:
   for each (u,v,w):
     若 w=1: g[u]←v, rg[v]←u
     若 w=2: g[u]←v, g[v]←u, 同时 rg[u]←v, rg[v]←u
3. 正向 BFS (bfs_min):
   mn[1..n] = INF, mn[1] = a[1]
   q = {1}
   while q 非空:
     u = q.pop()
     for v in g[u]:
       w = min(mn[u], a[v])
       若 mn[v] > w: mn[v]=w, q.push(v)
4. 反向 BFS (bfs_max):
   mx[1..n] = -INF, mx[n] = a[n]
   q = {n}
   while q 非空:
     u = q.pop()
     for v in rg[u]:    // 注意: 这里 rg 存的是"原图中能到达 u 的点"
       w = max(mx[u], a[v])
       若 mx[v] < w: mx[v]=w, q.push(v)
5. 答案:
   opt = 0
   for u in 1..n:
     if mn[u]≠INF and mx[u]≠-INF:
       opt = max(opt, mx[u] - mn[u])
   输出 opt
```

### 复杂度

- **时间**：$O\bigl(\Delta\cdot (n+m)\bigr)$，$\Delta\le 100$ 为价格值域，实际运行接近线性
- **空间**：$O(n+m)$ 存图

### 正确性

由引理 1 和定理 2 保证。正向传播正确计算出每个节点的 $\operatorname{mn}$，反向传播对称计算出 $\operatorname{mx}$，取 $\max(\operatorname{mx}-\operatorname{mn})$ 即为最优差价。

---

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};
constexpr int INF{1145141919};

int n,m,a[mxn],mn[mxn],mx[mxn];
vector<int>g[mxn],rg[mxn];

void bfs_min(int s)
{
	fill_n(mn,n,INF);
	mn[s]=a[s];
	queue<int>q;
	q.push(s);
	while(!q.empty())
	{
		int u=q.front();q.pop();
		for(int v:g[u])
		{
			int w=min(mn[u],a[v]);
			if(mn[v]>w)
				mn[v]=w,q.push(v);
		}
	}
}

void bfs_max(int s)
{
	fill_n(mx,n,-INF);
	mx[s]=a[s];
	queue<int>q;
	q.push(s);
	while(!q.empty())
	{
		int u=q.front();q.pop();
		for(int v:rg[u])
		{
			int w=max(mx[u],a[v]);
			if(mx[v]<w)
				mx[v]=w,q.push(v);
		}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int i=0,u,v,w;i<m;++i)
	{
		cin>>u>>v>>w;--u;--v;
		g[u].push_back(v);
		rg[v].push_back(u);
		if(w==2)
		{
			g[v].push_back(u);
			rg[u].push_back(v);
		}
	}

	bfs_min(0);
	bfs_max(n-1);

	int ans=0;
	for(int i=0;i<n;++i)
		if(mn[i]!=INF&&mx[i]!=-INF)
			ans=max(ans,mx[i]-mn[i]);
	cout<<ans<<'\n';

	return 0;
}
```
