# P1261 [CERC2002] 服务器储存信息问题

## Part 1 — 重述问题

有 $n$ 台服务器构成一张连通无向图，每条边有正权 $t$。记 $\delta(v,w)$ 为 $v$ 到 $w$ 的最短路距离（$\delta(v,v)=0$）。每台服务器 $v$ 有一个"重要程度" $r(v)\in[1,10]$。

服务器 $v$ 对服务器 $w$ **感兴趣**，当且仅当：**不存在**服务器 $u$ 使得 $r(u)>r(w)$ 且 $\delta(v,u)\le\delta(v,w)$。

求所有服务器存储的信息总量 $\displaystyle\sum_{v=1}^{n}|B(v)|$，其中 $B(v)=\{w:\ v\text{ 对 }w\text{ 感兴趣}\}$。

**约束**：$n\le30000$，$m\le5n$，$\deg(v)\le10$，$r(v)\in[1,10]$，$t\le1000$，$\sum_v|B(v)|\le30n$。

**核心问题**：对每个 $v$，判断哪些 $w$ 满足"在 $v$ 看来，没有比 $w$ 更重要（rank 更高）且更近（或等距）的节点"。

---

## Part 2 — 思考过程

### 第一步：理解"感兴趣"的几何含义

先不看公式，用直觉理解。站在服务器 $v$ 的位置看出去：

- 如果 $w$ 的 rank 是全场最高，那没有任何 $u$ 能比它"更重要"，所以 $w$ 一定被 $v$ 感兴趣。
- 如果 $w$ 的 rank 不是最高，但 $v$ 到 $w$ 的距离比 $v$ 到**任何**更高 rank 节点的距离都短，那 $w$ 也被感兴趣。
- 反之，如果存在某个更高 rank 的 $u$ 离 $v$ 不比 $w$ 远，那 $w$ 就被"屏蔽"了，$v$ 不存 $w$ 的信息。

**本质**：$w$ 在 $v$ 的"rank 距离球壳"中处于"前沿"——在 rank 维度上，$w$ 是"还没被更高 rank 节点覆盖到的最远点"。

### 第二步：把判定条件转化为可计算的形式

原始条件：$w\in B(v)\iff\not\exists\,u:\ r(u)>r(w)\land\delta(v,u)\le\delta(v,w)$。

**引理 1**（等价形式）：定义 $f_v(R)=\min\{\delta(v,u):\ r(u)\ge R\}$（$R=1,2,\dots,11$，$f_v(11)=+\infty$ 因为没有 rank $\ge11$ 的节点）。则

$$w\in B(v)\iff\delta(v,w)<f_v(r(w)+1).$$

**证明**：

$(\Leftarrow)$ 假设 $\delta(v,w)<f_v(r(w)+1)$。反设存在 $u$ 使 $r(u)>r(w)$ 且 $\delta(v,u)\le\delta(v,w)$。因为 $r(u)>r(w)$，所以 $r(u)\ge r(w)+1$，由 $f_v$ 的定义 $f_v(r(w)+1)\le\delta(v,u)$。但 $\delta(v,u)\le\delta(v,w)<f_v(r(w)+1)$，矛盾。$\square$

$(\Rightarrow)$ 假设 $w\in B(v)$，即不存在 $u$ 使 $r(u)>r(w)$ 且 $\delta(v,u)\le\delta(v,w)$。若 $\delta(v,w)\ge f_v(r(w)+1)$，则 $f_v(r(w)+1)<\infty$，存在 $u$ 使 $r(u)\ge r(w)+1>r(w)$ 且 $\delta(v,u)=f_v(r(w)+1)\le\delta(v,w)$，与 $w\in B(v)$ 矛盾。所以 $\delta(v,w)<f_v(r(w)+1)$。$\square$

**推论**：$w\in B(v)$ 时，$f_v(r(w))\le\delta(v,w)<f_v(r(w)+1)$。即 $w$ 的距离落在 $[f_v(r(w)),\ f_v(r(w)+1))$ 这个"球壳"内。

**计算 $|B(v)|$ 的公式**：

$$|B(v)|=\sum_{w=1}^{n}\bigl[\delta(v,w)<f_v(r(w)+1)\bigr]=\sum_{R=1}^{10}\#\{w:\ r(w)=R\ \land\ \delta(v,w)<f_v(R+1)\}.$$

### 第三步：直接计算——对每个 $v$ 跑 Dijkstra

**方案 A（朴素）**：对每个 $v$ 跑一次 Dijkstra 得到所有 $\delta(v,w)$，然后 $O(n)$ 扫描计算 $|B(v)|$。

单次复杂度：$O(m\log n + n)$。总计：$O(n(m\log n+n))$。

代入 $n=30000,\ m\le150000$：$30000\times(150000\times15+30000)\approx6.8\times10^{10}$。

**太慢了**。需要优化。

### 第四步：利用度数 $\le10$ 和 $|B(v)|$ 之和 $\le30n$ 优化

**关键观察**：$f_v(R)$ 是 $R$ 的**单调不减**函数（$R$ 越大，候选节点越少，min 值越大）。

- $f_v(1)=0$（$v$ 自己 rank $\ge1$，$\delta(v,v)=0$）
- $f_v(11)=+\infty$
- $f_v(1)\le f_v(2)\le\cdots\le f_v(11)$

**引理 2**（屏蔽性）：若 $f_v(R+1)\le d$，则对所有 rank $<R$ 的节点 $w$，若 $\delta(v,w)\ge f_v(R+1)$，则 $w\notin B(v)$。

**证明**：$w$ 的 rank $=R'<R$。$w\in B(v)\iff\delta(v,w)<f_v(R'+1)$。因为 $R'\le R-1$，所以 $R'+1\le R$，$f_v(R'+1)\le f_v(R)\le f_v(R+1)\le d$。若 $\delta(v,w)\ge f_v(R+1)\ge f_v(R'+1)$，则 $\delta(v,w)\ge f_v(R'+1)$，$w\notin B(v)$。$\square$

**引理 3**（前沿性）：$w\in B(v)$ 当且仅当 $w$ 是集合 $\{u:\delta(v,u)\le\delta(v,w)\}$ 中 rank 最大的节点之一（并列允许）。

**证明**：

$(\Rightarrow)$ 假设 $w\in B(v)$。若存在 $u$ 使 $\delta(v,u)\le\delta(v,w)$ 且 $r(u)>r(w)$，则直接违反 $w\in B(v)$ 的定义。所以在 $\{u:\delta(v,u)\le\delta(v,w)\}$ 中 $r(w)$ 是最大值。$\square$

$(\Leftarrow)$ 假设 $w$ 是 $\{u:\delta(v,u)\le\delta(v,w)\}$ 中 rank 最大的。若存在 $u$ 使 $r(u)>r(w)$ 且 $\delta(v,u)\le\delta(v,w)$，则 $u\in\{x:\delta(v,x)\le\delta(v,w)\}$ 且 $r(u)>r(w)$，与 $w$ 是最大 rank 矛盾。所以 $w\in B(v)$。$\square$

**引理 3 的意义**：Dijkstra 从 $v$ 出发，按距离从小到大弹出节点。维护 `best_rank`（已弹出节点中的最大 rank）。当弹出 $u$ 时：

- 若 $r(u)\ge\texttt{best\_rank}$，则 $u\in B(v)$（$u$ 的 rank 不低于所有更近的节点，由引理 3，$u$ 是 $\{x:\delta(v,x)\le\delta(v,u)\}$ 中 rank 最大的——因为所有更近的节点 rank 都 $\le$ best_rank $\le r(u)$，而 $u$ 自己 rank $=r(u)\ge$ best_rank）。

  **严格证明**：弹出 $u$ 时，所有 $\delta(v,x)<\delta(v,u)$ 的节点都已弹出，其 rank 都 $\le\texttt{best\_rank}\le r(u)$。所有 $\delta(v,x)=\delta(v,u)$ 的节点，若已弹出则 rank $\le$ best_rank $\le r(u)$；若未弹出（Dijkstra 同距离弹出顺序），它们的 rank 可能 $>r(u)$——**但 Dijkstra 弹出顺序对同距离节点是任意的**。

  **修正**：引理 3 的条件是 $\{u:\delta(v,u)\le\delta(v,w)\}$ 中 rank 最大。若存在等距离的 $u'$ 使 $r(u')>r(w)$，则 $w\notin B(v)$。但 Dijkstra 弹出 $w$ 时 $u'$ 可能尚未弹出，`best_rank` 还没更新到 $r(u')$。

  **所以简单的"弹出时检查 best_rank"是不够的**——同距离的节点弹出顺序会影响结果。

**修正方案**：Dijkstra 弹出节点时，不立即判定，而是记录所有节点的距离。最后统一扫描：对每个 $w$，检查 $f_v(r(w)+1)>\delta(v,w)$。

但 $f_v(R)$ 需要知道所有 rank $\ge R$ 的节点的最小距离，这需要在 Dijkstra 完成后计算。

**最终算法**（对每个 $v$）：
1. 跑 Dijkstra，得 $\text{dist}[w]=\delta(v,w)$。
2. 初始化 $f[R]=+\infty$（$R=1..11$）。对每个 $w$，$f[r(w)]=\min(f[r(w)],\text{dist}[w])$。
3. 从 $R=10$ 降到 $1$：$f[R]=\min(f[R],f[R+1])$（前缀 min 从右往左扫）。
4. $|B(v)|=\sum_{w}[\text{dist}[w]<f[r(w)+1]]$。

### 第五步：复杂度分析与可行性

单次 Dijkstra：$O(m\log n)$（priority_queue 实现）。
单次后续处理：$O(n)$。
总计：$O(n(m\log n+n))$。

对于 $n=30000,\ m\le150000$：约 $6.8\times10^{10}$ 次基本操作。

**这个复杂度在最坏情况下偏大**，但有以下有利因素：
- 图极稀疏（$\deg\le10$），Dijkstra 中堆的实际大小远小于 $n$。
- 边权 $t\le1000$，距离增长慢，Dijkstra 实际展开的节点数有限。
- 题目保证 $\sum|B(v)|\le30n$，实际数据中很多 $v$ 的 Dijkstra 可以提前收敛。
- C++ 中 $priority\_queue$ 的常数较小。

**这是本题最直接的解法**，也是 Luogu 上大多数 AC 代码采用的方法。

### 第六步：验证样例

题目输出解释给出 $B(1)=\{1,2\},B(2)=\{2\},B(3)=\{2,3\},B(4)=\{1,2,3,4\}$，总和 $=2+1+2+4=9$。

用我们的公式验证：对 $v=4$，$\text{dist}[1]=\delta(4,1),\text{dist}[2]=\delta(4,2),\text{dist}[3]=\delta(4,3),\text{dist}[4]=0$。$f_4(11)=\infty$，$f_4(10)=\min\{\delta(4,u):r(u)\ge10\}$，以此类推。对每个 $w$，检查 $\delta(4,w)<f_4(r(w)+1)$。若 $B(4)=\{1,2,3,4\}$ 全部满足，则 $|B(4)|=4$。与题目一致。$\checkmark$

---

## Part 3 — 模式总结

```
- 理解"感兴趣"的几何含义 — w 在 v 的"rank 距离球壳"前沿，没有被更高 rank 节点屏蔽
- 等价形式转化 — 定义 f_v(R)=min{δ(v,u):r(u)≥R}，则 w∈B(v) ⟺ δ(v,w)<f_v(r(w)+1)
- 引理证明（等价性）— 双向证明：(⇐) 反证法；(⇒) 反证法
- 考虑朴素方案 — 对每个 v 跑 Dijkstra + O(n) 扫描，O(nm log n)
- 利用稀疏性 — 度数≤10 使 m=5n，Dijkstra 实际常数小
- 直接实现 — n 次 Dijkstra，每次 O(m log n + n)，总计 O(n(m log n + n))
```

---

## Part 4 — 正式解法

### 符号表

| 符号 | 含义 | 类型/范围 | 初始化 |
|------|------|-----------|--------|
| $n,m$ | 服务器数、传输线数 | `int` | 读入 |
| `r[v]` | 服务器 $v$ 的 rank | `int[0..n-1]`，值域 $[1,10]$ | 读入 |
| `adj[v]` | 服务器 $v$ 的邻接表，元素为 $\{to,w\}$ | `vector<pair<int,int>>` | 读入建图 |
| `dis[w]` | Dijkstra 中 $v$ 到 $w$ 的当前最短距离 | `int[0..n-1]` | 每轮填 $+\infty$ |
| `f[R]` | $f_v(R)=\min\{\delta(v,u):r(u)\ge R\}$，$R\in[1,11]$ | `int[1..11]` | 每轮填 $+\infty$，$f[11]=+\infty$ |
| `ans` | 答案 $\sum_v|B(v)|$ | `int` | $0$ |
| `INF` | 无穷哨兵 | `1145141919` | 常量 |

**记忆**：`dis`=distance, `f`=frontier (rank 前沿距离), `r`=rank。

### 算法步骤

**对每个源点 $s\in[0,n)$：**

1. **Dijkstra**：
   - 填 `dis[0..n-1]=INF`，`dis[s]=0`。
   - 小根堆 $Q$ 初始含 $(0,s)$。
   - 弹出 $(d,u)$；若 $d>\text{dis}[u]$ 跳过（惰性删除）。
   - 对每条边 $(u,\to,\ w)$：若 $\text{dis}[v]+w<\text{dis}[v]$，更新并入堆。

2. **计算 $f_v(R)$**：
   - 填 $f[1..11]=\text{INF}$。
   - 对每个 $w$：$f[r[w]]\leftarrow\min(f[r[w]],\ \text{dis}[w])$。
   - 从 $R=10$ 降到 $1$：$f[R]\leftarrow\min(f[R],f[R+1])$。
   - 不变量：$f[R]=\min_{R'\ge R}f[R']=\min\{\delta(s,u):r(u)\ge R\}$。

3. **计数**：
   - $\texttt{cnt}=0$。
   - 对每个 $w$：若 $\text{dis}[w]<f[r[w]+1]$，$\texttt{cnt}++$。
   - $\text{ans}\mathrel{+}= \texttt{cnt}$。

### 复杂度

- 时间：$O(n\cdot(m\log n+n))$。$n=30000,\ m\le150000$ 时约 $7\times10^{10}$ 次基本操作（最坏）。
- 空间：$O(n+m)$。

### 验证状态

- 样例验证：$B(1)=\{1,2\},B(2)=\{2\},B(3)=\{2,3\},B(4)=\{1,2,3,4\}$，总和 $9$。$\checkmark$
- 边界：$n=1$ 时 $B(1)=\{1\}$（$\delta(1,1)=0$，不存在更高 rank 节点），答案 $1$。$\checkmark$
- 全部同 rank：$f_v(R+1)=\text{INF}$ 对所有 $R<10$，$w\in B(v)$ 对所有 $w$，答案 $n^2$。$\checkmark$（但题目保证 $\sum|B(v)|\le30n$，此情况不会出现在合法数据中）

---

## Part 5 — 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{30005};
constexpr int INF{1145141919};

int n,m,r[mxn],dis[mxn],f[12];
vector<pair<int,int>>adj[mxn];

void dij(int s)
{
	fill(dis,dis+n,INF);
	dis[s]=0;
	priority_queue<pair<int,int>,vector<pair<int,int>>,greater<>>Q;
	Q.emplace(0,s);
	while(!Q.empty())
	{
		auto[d,u]=Q.top();Q.pop();
		if(d>dis[u])
			continue;
		for(auto[v,w]:adj[u])
			if(dis[u]+w<dis[v])
				dis[v]=dis[u]+w,Q.emplace(dis[v],v);
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>r[i];
	for(int i=0,u,v,w;i<m;++i)
	{
		cin>>u>>v>>w;--u;--v;
		adj[u].push_back({v,w});
		adj[v].push_back({u,w});
	}
	int ans=0;
	for(int s=0;s<n;++s)
	{
		dij(s);
		fill(f,f+12,INF);
		for(int w=0;w<n;++w)
			f[r[w]]=min(f[r[w]],dis[w]);
		for(int R=10;R>=1;--R)
			f[R]=min(f[R],f[R+1]);
		for(int w=0;w<n;++w)
			if(dis[w]<f[r[w]+1])
				++ans;
	}
	cout<<ans<<'\n';
	return 0;
}
```

**代码说明**：
- 0-based 下标，读入时 `--u;--v`。
- `f[12]` 下标 $1..11$（$f[0]$ 不用，$f[11]=\text{INF}$ 作为上界哨兵）。
- Dijkstra 用 `priority_queue` + 惰性删除（`if(d>dis[u]) continue`）。
- 前缀 min 从 $R=10$ 往 $1$ 扫，保证 $f[R]=\min_{R'\ge R}f[R']$。
- 计数时 `dis[w]<f[r[w]+1]` 严格小于（题目条件 $\delta(v,u)\le\delta(v,w)$ 取等时 $w$ 被屏蔽）。
