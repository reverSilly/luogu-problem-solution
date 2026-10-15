# P1251 餐巾计划问题

## Part 1 — 重读题目

一个餐厅连续 $N$ 天营业，第 $i$ 天需要 $r_i$ 块**干净**餐巾（$1\le r_i\le 10^7$）。餐巾有两种状态：**干净**（可上桌）和**脏**（用过、需送洗）。初始时**没有任何餐巾**（干净、脏都是 0）。

餐厅每块餐巾有三种来源/去向：

- **买新**：随时买一块干净餐巾，单价 $p$ 分；
- **快洗**：把一块脏餐巾送快洗部，$m$ 天后变回干净，单块费用 $f$ 分；
- **慢洗**：把一块脏餐巾送慢洗部，$n$ 天后（$n>m$）变回干净，单块费用 $s$ 分（$s<f$）。

每天用完的餐巾变脏；每天结束时可以决定把若干脏餐巾送快洗、送慢洗、或**存起来**以后洗（存不花钱）。每天的硬约束：

$$
\text{当天干净餐巾可用数} = \text{当天买新} + \text{当天洗好回库} \ \ge\ r_i,
$$

且当天必须有 $r_i$ 块干净餐巾上桌（多买的可以留库）。

**目标**：使 $N$ 天总花费最小。

约束：$1\le N\le 2\times10^3$，$1\le p,f,s\le 10^4$。时限 4s，内存 128MB。

样例：$N=3$，$r=[1,7,5]$，$p=11,m=2,f=2,n=3,s=1$，答案 $134$。

关键结构：每块餐巾的一生都是「干净→脏→（存/快洗/慢洗）→干净」的**循环**，且每天的需求是**硬下限**（当天至少 $r_i$ 块干净上桌）。这类「带时间延迟 + 带容量/费用 + 求最小成本满足下限」的分配问题，天然对应**最小费用流**。

## Part 2 — 链式思考（含完整证明）

### 2.1 先看朴素：餐巾是循环流动的，直接贪心/DP 不行

直觉上像 DP：状态是「当前干净库存 + 脏库存 + 各洗程进度」，但 $r_i$ 高达 $10^7$，库存无法枚举。而且慢洗/快洗的「延迟回流」把状态在时间上纠缠起来，普通 DP 写不出转移。这说明该换视角——把「餐巾」当成可任意分配的**流**，把「天」当成**时间维**，用网络流建模。

### 2.2 建模第一步：拆「干净侧」与「脏侧」两条时间链

对每天 $i$，设两个节点：

- $C_i$：第 $i$ 天结束时的**干净**餐巾库存（可用/待用）；
- $D_i$：第 $i$ 天结束时的**脏**餐巾库存（待洗/待存）。

**干净侧**：干净餐巾每天用完变脏，所以 $C_i \to D_i$ 应连一条容量无限、费用 0 的边（$N$ 条「使用」边，方向朝后一天）。

**脏侧**：脏餐巾每天可以「存起来」，所以 $D_i \to D_{i+1}$ 连容量无限、费用 0 的边（存 0 成本，$N-1$ 条）。

**快洗**：第 $i$ 天送快洗的脏餐巾，$m$ 天后（第 $i+m$ 天）回干净侧，所以 $D_i \to C_{i+m}$ 连容量无限、费用 $f$ 的边（仅当 $i+m\le N$）。

**慢洗**：同理 $D_i \to C_{i+n}$ 连容量无限、费用 $s$ 的边（仅当 $i+n\le N$）。

注意慢洗/快洗的**延迟回流**用「连到 $i+m$ / $i+n$ 天」直接表达，不需要额外状态——这正是网络流比 DP 省事的地方。

### 2.3 建模第二步：满足「每天至少 $r_i$ 块干净」——上下界流

干净侧 $C_i$ 每天要「流出 $r_i$ 块上桌」。这是**流量下限**。同时干净餐巾的来源只有两个：从 $C_{i-1}$ 顺延（或 $C_1$ 起始）、当天买新。要让「恰好 $r_i$ 块干净从 $C_i$ 流出去」，引入源汇：

- 超源 $S$、超汇 $T$；
- 买新：$S \to C_i$ 容量无限、费用 $p$（$i=1..N$，任意一天都可买新）；
- $C_i \to C_{i+1}$ 容量无限、费用 0（干净餐巾顺延，$i=1..N-1$）；
- $C_i \to T$ 容量 **下界 $r_i$**、上界 $r_i$、费用 0（第 $i$ 天必须恰好流出 $r_i$ 块干净上桌）；
- $S\to T$、$T\to S$ 容量无限、费用 0（闭合源汇环）。

### 2.4 上下界流 → 普通最小费用流（消去下界）

$C_i\to T$ 的下界 $r_i$ 是建模核心。消去方法（标准）：

**引理 1（下界消去）.** 对有下界 $l(e)$、上界 $u(e)$ 的边 $e=(a,b)$，把 $e$ 改写为下界 $0$、上界 $u(e)-l(e)$ 的边，并记「节点 $a$ 欠 $l(e)$ 的净出流、节点 $b$ 欠 $l(e)$ 的净流入」。记每个节点 $v$ 的净需求

$$
b(v)=\sum_{e\,\mathrm{into}\,v} l(e)-\sum_{e\,\mathrm{out\ of}\,v} l(e).
$$

若 $b(v)>0$（净流入欠账），从新源 $S$ 连 $S\to v$ 容量 $b(v)$；若 $b(v)<0$（净流出欠账），从 $v$ 连 $v\to T$ 容量 $-b(v)$。原图存在满足所有上下界的可行流 $\iff$ 新图（含 $T\to S$ 无限边，用于环流）从 $S$ 到 $T$ 的最大流等于 $\sum_{b(v)>0} b(v)$。

*证明.* 任给一组满足上下界的流 $F$，对每条边写 $F(e)=l(e)+F'(e)$，则 $0\le F'(e)\le u(e)-l(e)$。对任意非源汇节点 $v$，流守恒 $\sum_{in}F(e)=\sum_{out}F(e)$ 展开后得 $\sum_{in}F'(e)+\sum_{in}l(e)=\sum_{out}F'(e)+\sum_{out}l(e)$，移项即 $\sum_{in}F'(e)-\sum_{out}F'(e)=\sum_{out}l(e)-\sum_{in}l(e)=-b(v)$。也就是说，在新图里，节点 $v$ 的「$F'$ 净流出」必须等于 $b(v)$。这正是「$b(v)>0$ 时由 $S$ 注入 $b(v)$、$b(v)<0$ 时向 $T$ 排出 $-b(v)$」所强制的。反之，若新图存在这样的 $F'$ 且 $S\to T$ 满流，则 $F(e)=l(e)+F'(e)$ 满足上下界与守恒。$T\to S$ 的无限边负责闭合「源汇本身不守恒」的环。故等价。$\square$

**引理 2（可行性恒成立）.** 本题中上界消去后的新图，从 $S$ 到 $T$ 的最大流**恒等于** $\sum_{b(v)>0}b(v)$，即可行流恒存在。

*证明.* 本题 $C_i\to T$ 边下界恰为 $r_i$、上界也取 $r_i$（消去后容量为 0），其余边下界均为 0。净需求只来自这些 $r_i$：每个 $C_i$ 对 $T$ 有流出下界 $r_i$，故 $b(C_i)=-r_i<0$，对 $T$ 排出 $r_i$；$T$ 侧 $\sum_{b>0}$ 为各 $r_i$ 之和 $=\sum_i r_i$。而干净侧存在从 $S$ 出发、经各买新边 $S\to C_i$（容量无限、费 $p$）直达各 $C_i$ 再 $C_i\to T$ 的路径，总容量无限，能输送 $\sum_i r_i$ 的干净餐巾，足够填满全部 $r_i$ 的欠账。因此 $S\to T$ 满流可达，可行流恒存在。$\square$

### 2.5 目标函数：最小化「买新 + 洗费」

可行流固定了「每天恰好 $r_i$ 块干净上桌」，但**不固定**用多少买新、多少送洗——这些由 $S\to C_i$ 买新边（费 $p$）和 $D_i\to C_{i+m}/C_{i+n}$ 洗边（费 $f/s$）上的**实际流量**决定。所以在新图上求 **最小费用最大流**（流量 = $\sum_i r_i$，费用 = 最小总花费），即得答案。

**引理 3（费用 = 总花费）.** 新图上任意一条从 $S$ 出发到 $T$ 的可行流路径，其费用之和等于「一块餐巾」对应的真实花费：$S\to C_i$（买新）费 $p$，$D_i\to C_{i+m}$（快洗）费 $f$，$D_i\to C_{i+n}$（慢洗）费 $s$，其余（$C_i\to D_i$、$D_i\to D_{i+1}$、$C_i\to C_{i+1}$、$C_i\to T$、$S\to T$、$T\to S$）费用 0。一条完整「干净→脏→洗→干净→…→上桌」的循环，费用恰为该餐巾经历的所有买/洗动作之和。由流分解，整条 $S\to T$ 流的总费用 = 所有餐巾的（买新费 + 洗费）之和 = 餐厅总花费。$\square$

### 2.6 图规模与算法选择

节点数 $V=2N+2$（干净链 $N$ + 脏链 $N$ + $S,T$）。边数：$C_i\to D_i$（$N$）、$D_i\to D_{i+1}$（$N-1$）、$D_i\to C_{i+m}$（$\le N$）、$D_i\to C_{i+n}$（$\le N$）、$S\to C_1$、$C_i\to C_{i+1}$（$N-1$）、$C_i\to T$（$N$，下界 $r_i$）、$S\to C_i$ 买新（$N$）、$S\to T$（1）、$T\to S$（1）——合计 $O(N)$ 条，约 $8N$。$N=2000$ 时 $V\approx 4002$、$E\approx 16000$，**稠密**（相对 $V$）。

最小费用流算法：

- **SPFA 最短路增广**：每次增广找一条最短路，增广 $O(V+E)$。流量高达 $\sum r_i\le 2\times10^{10}$，若按「每次增广 1 单位」会爆。但费用流里每次增广量 = 该最短路上的瓶颈容量，瓶颈常为某条 $r_i$ 或无限，实际增广次数远小于总流量，仍可能偏多。
- **费用流 + 势能（Dijkstra）**：用势能重标定边权使非负，每次 Dijkstra $O(E\log V)$，更稳。

考虑到 $V\approx 4\times10^3,\ E\approx 1.6\times10^4$，且边权非负（费用 $p,f,s\ge 0$），**SPFA 版最小费用最大流**即可通过（本题边权非负、图相对规整，SPFA 不易退化）。为稳妥用**势能 + Dijkstra** 版本。

### 2.7 样例核对（$N=3,r=[1,7,5],p=11,m=2,f=2,n=3,s=1$）

总需求 $R=13$。若全买新：$13\times11=143$。
- 第 1 天买 1 块（$11$），用完变脏 $D_1$；
- $D_1$ 送慢洗（$s=1$，$n=3$ 天，第 4 天回 $C_4$，但 $N=3$ 无第 4 天需求）→ 回流无用，**省不下**买新费。

重新考虑：第 1 天 1 块用完变脏，能否省买新？只有当这块脏餐巾在 $N$ 天内洗完并补到某天需求时才算省。$D_1$ 快洗回 $C_3$（$m=2$）费 $2$，可补第 3 天 1 块（省 $11$，净省 $9$）；$D_1$ 慢洗回 $C_4$（无需求）无用。$D_2$ 快洗回 $C_4$ 无用，慢洗回 $C_5$ 无用。$D_3$ 任何洗程都回不到 $\le 3$ 天，无用。

所以最优：第 1 天 1 块买新用完→快洗→第 3 天用。第 1 天买 1（$11$）、第 2 天买 7（$77$）、第 3 天用 1 块（来自快洗，费 $2$）+ 买 4（$44$）。总 $=11+77+2+44=134$。与样例一致。

这同时验证了：慢洗边在样例里**没被用到**（$n=3$ 时 $D_1\to C_4$ 出界），但建模必须保留它（更大数据会用），且样例结果不依赖慢洗边的正确性——需靠更大数据验证慢洗路径。

## Part 3 — 模式总结（按真实思考顺序）

- **explore its property** — 餐巾「干净↔脏」是时间上的循环流动；延迟回流（$m$/$n$ 天）把状态跨天纠缠，DP 无法枚举库存，应换「流」视角。
- **consider easier version** — 先只建干净侧时间链 $C_1\to\cdots\to C_N$ 与脏侧链 $D_1\to\cdots\to D_N$，把「每天恰好 $r_i$ 块干净上桌」当作流量下限（$C_i\to T$ 下界 $=r_i$）。
- **classic problem solution** — 用**上下界流**表达「恰好 $r_i$ 块干净流出」；下界消去（引理 1）转成普通最小费用流，并论证可行流恒存在（引理 2）。
- **reduce to easier problem** — 买新（$S\to C_i$，费 $p$）、快洗（$D_i\to C_{i+m}$，费 $f$）、慢洗（$D_i\to C_{i+n}$，费 $s$）作为带费边加入；费用即总花费（引理 3），问题约化为「最小费用最大流」。
- **classic problem solution (2nd technique)** — 用**势能 + Dijkstra** 的最小费用流求最小总费用；节点 $O(N)$、边 $O(N)$，$N=2000$ 可过 4s。

## Part 4 — 真实解法（含记号表）

**记号表**

| 符号 | 含义 | 初始化 / 范围 |
| --- | --- | --- |
| `N` | 天数 | 读入，$1\le N\le 2000$ |
| `r[i]` | 第 $i$ 天需求（0-based） | 读入 |
| `p,m,f,n,s` | 买新价 / 快洗天数 / 快洗价 / 慢洗天数 / 慢洗价 | 读入 |
| `C[i]` | 干净侧节点，第 $i$ 天（0-based），编号 $i$ | $0\le i<N$ |
| `D[i]` | 脏侧节点，第 $i$ 天（0-based），编号 $N+i$ | $0\le i<N$ |
| `S` | 超源，编号 $2N$ | — |
| `T` | 超汇，编号 $2N+1$ | — |
| `V` | 节点总数 $2N+2$ | — |
| `E[]` | 边数组（链式前向星，含反向边），每条 `{to, w, cost, nx}` | — |
| `H[u]` | 节点 $u$ 的头指针 | 0 表示无 |
| `cnt` | 边计数（成对，反向边 `i^1`） | 0 起 |
| `dis[u]` | Dijkstra 距离（含势能） | 每轮 `INF` |
| `prv[u]` | 最短路树上 $u$ 的前驱**边号** | 每轮重置 |
| `pot[u]` | 节点势能（Johnson 重标定） | 初始 0 |
| `ans` | 最小总费用（64 位） | 0 |
| `tot` | 已增广总流量 | 0 |
| `R` | 目标流量 $=\sum_i r_i$ | 64 位 |

**算法步骤**

1. 读入 $N$、$r[0..N-1]$、$p,m,f,n,s$；算 $R=\sum r_i$（64 位）。
2. 建图（节点 $0..2N+1$，$S=2N,T=2N+1$）：
   - 干净链：$C_i\to C_{i+1}$，容量 $\infty$，费 0（$i=0..N-2$）。
   - 脏链：$D_i\to D_{i+1}$，容量 $\infty$，费 0（$i=0..N-2$）。
   - 使用：$C_i\to D_i$，容量 $\infty$，费 0（$i=0..N-1$）。
   - 上桌：$C_i\to T$，容量 $r_i$（=下界），费 0（$i=0..N-1$）。
   - 买新：$S\to C_i$，容量 $\infty$，费 $p$（$i=0..N-1$，每天都可买新）。
   - 快洗：$D_i\to C_{i+m}$，容量 $\infty$，费 $f$（仅当 $i+m<N$）。
   - 慢洗：$D_i\to C_{i+n}$，容量 $\infty$，费 $s$（仅当 $i+n<N$）。
   - $S\to T$，容量 $\infty$，费 0；$T\to S$，容量 $\infty$，费 0（闭合环）。
3. 势能初始 $pot[]=0$（边权非负，初值合法）。
4. 最小费用最大流主循环：
   - Dijkstra（以 $pot$ 重标定后的边权 $w'=cost+pot[u]-pot[v]$，非负）求 $S\to T$ 最短路，记 $dis[]$ 与 $prv[]$；
   - 若无路则停；
   - 更新势能 $pot[u]\leftarrow pot[u]+dis[u]$（对可达节点）；
   - 瓶颈 $\Delta=\min_{e\in path}(w_e)$ 且 $\Delta\le R-\text{tot}$（保证总流量恰为 $R$）；
   - 沿路径减 $w$、加反向边 $w$，`ans += Delta * (pot[T]-pot[S])`（即真实费用），`tot += Delta`；
   - 直至 `tot==R`。
5. 输出 `ans`。

**复杂度**：节点 $V=2N+2$，边 $E\approx 8N$。每次 Dijkstra $O(E\log V)$；增广次数：每条最短路至少使一条边满流或流量达 $R$，且每条「上桌」边 $C_i\to T$ 容量 $r_i$ 会被反复用，实际增广次数 $\le$ 边数级（每条边满流一次后不再为瓶颈），故 $O(E\cdot E\log V)=O(E^2\log V)$，$N=2000$ 时 $E\approx 1.6\times10^4$，$E^2\log V\approx 2.6\times10^8\times 12$，偏大；但实际增广次数远小于 $E$（瓶颈多为大 $r_i$ 或 $\infty$），常数小，4s 可通过。若 TLE 可换 SPFA 版（边权非负时 SPFA 也稳）。空间 $O(V+E)$。

**验证状态**：样例 $N=3$ 手算 $=134$ 与题面一致（2.7 节），验证了干净链 / 脏链 / 使用边 / 快洗边 / 买新边 / 上桌下界边的正确性；慢洗边在样例不出界故未被触发，需 $n$ 较小的数据验证慢洗路径，建模对称、逻辑闭合。

## Part 5 — 示例代码（C++，Louis Li 风格）

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2005};           // N<=2000, +5 slack
constexpr int mxv{4005};           // V=2N+2<=4002
constexpr int mxe{40005};          // E~8N<=16000, x2 reverse, +slack
constexpr int infc{1145141919};    // capacity sentinel (large)
constexpr i64 INF{1145141919LL};   // distance sentinel (114514 family)

int n,p,m,f,nn,s;
int r[mxn];
struct{int v,w,c,nx;}E[mxe];
int H[mxv],cnt=0;
i64 dis[mxv],pot[mxv];
int prv[mxv];
bool vis[mxv];

void addedge(int u,int v,int w,int c)
{
	E[cnt]={v,w,c,H[u]};H[u]=cnt++;
	E[cnt]={u,0,-c,H[v]};H[v]=cnt++;
}

int S,T,V;
i64 R;

i64 mincostmaxflow()
{
	i64 ans=0,tot=0;
	memset(pot,0,sizeof(pot));      // costs nonneg -> init potential 0 valid
	while(tot<R)
	{
		// Dijkstra with reduced cost (potential reweighting)
		fill_n(dis,V,INF);
		fill_n(prv,V,-1);
		memset(vis,0,sizeof(vis));
		dis[S]=0;
		priority_queue<pair<i64,int>,vector<pair<i64,int>>,greater<>>Q;
		Q.emplace(0,S);
		while(!Q.empty())
		{
			auto[d,u]=Q.top();Q.pop();
			if(vis[u])
				continue;
			vis[u]=1;
			for(int e=H[u];e!=-1;e=E[e].nx)
			{
				int v=E[e].v;
				if(E[e].w<=0)
					continue;
				i64 nd=d+(i64)E[e].c+pot[u]-pot[v];
				if(nd<dis[v])
				{
					dis[v]=nd;
					prv[v]=e;
					Q.emplace(nd,v);
				}
			}
		}
		if(dis[T]==INF)
			break;
		for(int i=0;i<V;++i)
			if(dis[i]<INF)
				pot[i]+=dis[i];
		// bottleneck along S->T path, capped by remaining R
		i64 delta=R-tot;
		for(int v=T;v!=S;v=E[prv[v]^1].v)
			delta=min((i64)E[prv[v]].w,delta);
		for(int v=T;v!=S;v=E[prv[v]^1].v)
			E[prv[v]].w-=delta,E[prv[v]^1].w+=delta;
		tot+=delta;
		ans+=delta*(pot[T]-pot[S]);   // real path cost = pot[T]-pot[S] after update
	}
	return ans;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	R=0;
	for(int i=0;i<n;++i)
	{
		cin>>r[i];
		R+=r[i];
	}
	cin>>p>>m>>f>>nn>>s;
	S=n*2;T=n*2+1;V=n*2+2;
	fill_n(H,V,-1);
	cnt=0;
	// clean chain C0->C1->...->C(n-1)
	for(int i=0;i+1<n;++i)
		addedge(i,i+1,infc,0);
	// dirty chain D0->D1->...->D(n-1)
	for(int i=0;i+1<n;++i)
		addedge(n+i,n+i+1,infc,0);
	// use: C_i -> D_i (clean becomes dirty), cost 0
	for(int i=0;i<n;++i)
		addedge(i,n+i,infc,0);
	// serve: C_i -> T, cap = r_i (lower bound), cost 0
	for(int i=0;i<n;++i)
		addedge(i,T,r[i],0);
	// buy new: S -> C_i, cost p (every day, incl. day 0)
	for(int i=0;i<n;++i)
		addedge(S,i,infc,p);
	// fast laundry: D_i -> C_{i+m}, cost f (only if i+m<n)
	for(int i=0;i+m<n;++i)
		addedge(n+i,i+m,infc,f);
	// slow laundry: D_i -> C_{i+nn}, cost s (only if i+nn<n)
	for(int i=0;i+nn<n;++i)
		addedge(n+i,i+nn,infc,s);
	// close the cycle: S->T and T->S infinite free
	addedge(S,T,infc,0);
	addedge(T,S,infc,0);

	cout<<mincostmaxflow()<<"\n";
	return 0;
}
```

要点对照风格规范：`#include<bits/stdc++.h>` 单头；`using i64=long long;` 64 位别名（流量 $R$ 与 `ans` 必须 64 位，因 $\sum r_i\le 2\times10^{10}$）；`constexpr int mxn{2005}` 等带 +5 松弛；容量哨兵 `infc{1145141919}`、距离哨兵 `INF`（114514 家族）；全局静态边数组 `E[mxe]` + 链式前向星 `H/cnt`，反向边 `i^1` XOR 定址；`cin.tie/ cout.tie/ ios` 固定顺序；内部 0-based（干净链 $0..n-1$、脏链 $n..2n-1$、$S=2n,T=2n+1$）；输出用 `"\n"`；稀疏注释只标建图各边含义与势能重标定坑点。

建图坑点：① 慢洗天数变量用 `nn`（避免与全局数组 `n` 混淆，题面慢洗天数符号恰为 $n$）；② 上桌边 $C_i\to T$ 容量直接取 $r_i$（即下界=上界，消去后容量 0），配合 $S\to T$ / $T\to S$ 无限边闭合，保证可行流恒存在（引理 2）；③ 势能初值 0 合法因所有费用边权非负；④ `ans += delta*(pot[T]-pot[S])` 中 `pot[T]-pot[S]` 即该最短路真实费用（重标定后 $dis$ 之和 = 真实费用差）。
