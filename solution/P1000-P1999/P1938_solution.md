# P1938 [USACO09NOV] Job Hunt S

## 1. 重新读题

有 $C$ 座城市、$P$ 条**单向公路**（通行免费）、$F$ 条**单向航线**（票价 $T_i$）。奶牛从城市 $S$ 出发，初始身无分文。

规则：

- 每到达一座城市，可以在那里工作赚 $D$ 美元；同一城市每次到达都能再赚，往返次数不限；
- 没钱也能坐飞机，"用以后赚的钱付"，即允许欠账；
- 可以在任何城市、任何时刻退休。

求最多能赚多少钱；若可以无限赚，输出 $-1$。

### 关键约束与规模暗示

- $C\le220$，$P\le150$，$F\le350$，$D\le1000$，$T_i\le50000$。
- 点少、边少，允许 $O(C(P+F))$ 的 Bellman-Ford；不需要更快的算法。
- "往返次数不限"+"求最大收益"是典型的**正环**问题：存在可达的正收益环就能无限赚。

## 2. 思维链（Chain-of-thought）

### 2.1 把收益搬到边上

先明确"什么时候拿到 $D$"。

**建模**：把"到达城市 $v$"视为获得 $+D$。于是：

- 初始在城市 $S$，先把 $S$ 的 $D$ 赚了：$\text{dis}[S]=D$；
- 走公路 $u\to v$：到达 $v$ 赚 $D$，边权 $+D$；
- 坐航线 $u\to v$（票价 $t$）：到达 $v$ 赚 $D$，但要付 $t$，边权 $D-t$。

**引理 1（路径收益可加）**：对任何一条从 $S$ 出发的行走路线 $S=v_0\to v_1\to\dots\to v_k$，最终的钱数为

$$D+\sum_{i=1}^{k}\big(D-\text{cost}(v_{i-1}\to v_i)\big),$$

其中公路 $\text{cost}=0$、航线 $\text{cost}=T$。它只与"经过哪些边"有关，与中间顺序无关，且欠账不影响可行性。

**证明**：每到达一个城市加 $D$，每坐一次航线减 $T$，所有项相加即得；因为题面允许欠账，任何部分和是否为负都不构成限制。$\blacksquare$

所以只要记录"到达城市 $u$ 时手里的钱"这个量，做最长路即可。

### 2.2 先想：为什么会有无穷大

如果存在一条从 $S$ 可达的**正权环**（绕一圈收益 $>0$），就能绕它任意多次，收益无上界。

**引理 2（无正环时最优值是简单路径）**：若从 $S$ 可达的子图中不存在正权环，则任一最长行走都可化简为一条简单路径，且其收益不小于原行走。

**证明**：若行走中某个城市 $u$ 出现两次，把两次出现之间的那段拿出来，它是一个环，收益 $\le0$（题设无正环）。删掉这段环，从第一次到第二次出现 $u$ 的"位置/时间"直接接上，收益不减，行走变短。重复直到没有重复城市，得到简单路径。简单路径最多含 $C-1$ 条边。$\blacksquare$

于是无正环时，最优收益有上界 $C\cdot D$（最多经过 $C$ 个城市各赚一次，减去非负票价），可以在有限步内达到。

### 2.3 Bellman-Ford 求最长路

设 $\text{dis}[v]$ 为"从 $S$ 走到 $v$ 的最大收益"，初值 $\text{dis}[S]=D$，其余 $-\infty$。反复用所有边松弛：

$$\text{dis}[v]\leftarrow\max\big(\text{dis}[v],\ \text{dis}[u]+w(u,v)\big).$$

**引理 3（第 $k$ 轮的含义）**：做完 $k$ 轮松弛后，$\text{dis}[v]$ 不小于"从 $S$ 到 $v$、恰好/至多用 $k$ 条边的任意行走"的最大收益。

**证明**：对 $k$ 归纳。$k=0$ 时只有 $S$ 的 $0$ 条边行走，值为 $D$。设第 $k-1$ 轮成立；一条用 $\le k$ 条边的行走，其最后一条边为 $u\to v$，前缀用 $\le k-1$ 条边，由归纳假设第 $k-1$ 轮的 $\text{dis}[u]$ 已经不小于前缀收益，于是松弛边 $u\to v$ 后 $\text{dis}[v]$ 不小于整条行走收益。对所有边都松弛一遍即覆盖所有可能的前缀。$\blacksquare$

**引理 4（正环判定）**：$C-1$ 轮后若还能被松弛，则存在从 $S$ 可达的正权环；反之若存在可达正环，则 $C-1$ 轮后（甚至更早）必然还能被松弛。

**证明**：

- **正环 ⇒ 还能松弛**：反设 $C-1$ 轮后没有任何边可以松弛，即对所有边 $u\to v$ 都有 $\text{dis}[v]\ge\text{dis}[u]+w$。沿任意从 $S$ 出发的行走 $S=v_0\to\dots\to v_k$ 归纳可得 $\text{dis}[v_k]\ge$ 该行走的收益。但正环可达意味着存在收益无上界的行走（绕环足够多圈），而 $\text{dis}$ 是有限数，矛盾。所以必有边可松弛。
- **还能松弛 ⇒ 正环**：设 $C-1$ 轮后边 $u\to v$ 仍可改进。$\text{dis}[u]$ 对应某条 $\le C-1$ 条边的行走 $W$（引理 3 的构造性版本：松弛时记录来源即可）。$W$ 加上边 $u\to v$ 是一条 $\le C$ 条边的行走，且收益严格大于 $\text{dis}[v]$，而 $\text{dis}[v]$ 已不小于所有 $\le C-1$ 条边行走的收益。若这条 $\le C$ 边走中无重复城市，它就是简单路径、边数 $\le C-1$，矛盾。故必有重复城市，重复段是一个环。若该环收益 $\le0$，删去它得到一条更短、收益不更少的到 $v$ 的行走（仍 $\le C-1$ 边），与严格改进矛盾。因此该环收益 $>0$，即存在可达正环。$\blacksquare$

### 2.4 答案

- 若 $C-1$ 轮后仍能松弛：输出 $-1$。
- 否则答案 $=\max_v \text{dis}[v]$，$v$ 取所有可达城市（可以在任意城市退休）。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 把"到达城市赚 $D$"搬到边上，路径收益可加（引理 1）。
- **explore its property** — 无限收益 ⟺ 可达正环；无正环时最优解可化为简单路径（引理 2）。
- **classic problem solution** — Bellman-Ford 最长路跑 $C-1$ 轮，再判一次松弛（引理 4）。
- **special case** — 允许欠账，所以不需要限制 $\text{dis}\ge0$；答案取所有可达城市的最大值。

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `D` | `int` | 每城每次最多赚的钱 | 读入 | dollar |
| `P,C,F,S` | `int` | 公路数、城市数、航线数、起点 | 读入，$S$ 减一 | — |
| `e[]` | `edge[400]` | 所有边：`u,v,w` | 读入 | edges |
| `ecnt` | `int` | 边数 $P+F$ | $0$ | — |
| `dis[v]` | `int[230]` | 到达 $v$ 的最大收益 | `ninf`，`dis[S]=D` | distance/profit |
| `ninf` | `constexpr int` | 不可达哨兵 | `-1145141919` | 负无穷 |

### 4.2 算法步骤

1. 读入 $D,P,C,F,S$，把 $S$ 转为 0-based。
2. 读 $P$ 条公路，加入边 $(u,v,D)$。
3. 读 $F$ 条航线，加入边 $(u,v,D-T)$。
4. `dis` 全赋 `ninf`，`dis[S]=D`。
5. 做 $C-1$ 轮：每轮遍历所有边，若 `dis[u]!=ninf` 且 `dis[v]<dis[u]+w` 则更新；若整轮无更新提前退出。
6. 再扫一遍所有边，若仍可更新 → 输出 `-1`。
7. 否则输出 $\max_v\text{dis}[v]$。

### 4.3 复杂度

- 时间 $O(C(P+F))\le220\times500=1.1\times10^5$。
- 空间 $O(C+P+F)$。

## 5. 示例代码

```cpp
//
// 题目：P1938 [USACO09NOV] Job Hunt S
// https://www.luogu.com.cn/problem/P1938
//
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{230},mxm{400};
constexpr int ninf{-1145141919};

struct edge
{
	int u,v,w;
};

int D,P,C,F,S;
edge e[mxm];
int ecnt;
int dis[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>D>>P>>C>>F>>S;
	--S;
	for(int i=0;i<P;++i)
	{
		int u,v;
		cin>>u>>v;--u;--v;
		e[ecnt++]={u,v,D}; // 到达 v 赚 D
	}
	for(int i=0;i<F;++i)
	{
		int u,v,t;
		cin>>u>>v>>t;--u;--v;
		e[ecnt++]={u,v,D-t}; // 赚 D，付 t
	}
	fill(dis,dis+C,ninf);
	dis[S]=D;
	for(int it=0;it<C-1;++it)
	{
		bool upd=false;
		for(int i=0;i<ecnt;++i)
			if(dis[e[i].u]!=ninf&&dis[e[i].v]<dis[e[i].u]+e[i].w)
			{
				dis[e[i].v]=dis[e[i].u]+e[i].w;
				upd=true;
			}
		if(!upd)
			break;
	}
	bool inf=false;
	for(int i=0;i<ecnt;++i)
		if(dis[e[i].u]!=ninf&&dis[e[i].v]<dis[e[i].u]+e[i].w)
			inf=true;
	if(inf)
	{
		cout<<-1<<'\n';
		return 0;
	}
	int ans{0};
	for(int i=0;i<C;++i)
		ans=max(ans,dis[i]);
	cout<<ans<<'\n';
	return 0;
}
```
