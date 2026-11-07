# P1841 [JSOI2007] 重要的城市

> 难度：省选/NOI- · 标签：最短路、Floyd、支配关系

## 1. 重新读题

### 1.1 问题重述（人话版）

给一张 $N$ 个点、$M$ 条边的**无向正权**图（无重边、无自环）。称城市 $c$ 是重要的，如果删去 $c$（连同它的所有边）后，**存在**两个不同于 $c$ 的城市 $a,b$，使得 $a$ 到 $b$ 的最短距离变大，或者 $a,b$ 从连通变成不连通。要求按编号递增输出所有重要城市；一个都没有则输出 `No important cities.`。

### 1.2 关键约束与规模暗示

- $N\le 200$，$M\le \frac{N(N-1)}2$，边权 $0<c\le10^4$。$N$ 很小，$O(N^3)$ 的 Floyd 是标准起点；$O(N^4)$ 约 $1.6\times10^9$，偏危险。
- 边权全为正，这意味着最短路图是 DAG——这一点会在后面被反复使用。
- 需要「存在一对 $a,b$」，即只要有一对受影响就算数，不需要统计受影响的点对数。
- 输出按递增次序，说明可以对每个城市独立判断后统一输出。

## 2. 思维链（Chain-of-thought）

### 2.1 把「删点后距离变大」翻译成「所有最短路都经过它」

先明确一个等价关系。

**引理 1（重要的等价刻画）**：对城市 $c$ 和两个不同于 $c$ 的城市 $a,b$，删去 $c$ 后 $a\to b$ 的最短距离变大（或断开）**当且仅当**原图中**每一条** $a\to b$ 的最短路径都经过 $c$。

**证明**：
- 若每条最短路都经过 $c$：删去 $c$ 后，任何长度等于原最短距离的路径都不存在了（否则它是一条不经过 $c$ 的最短路，矛盾），所以最短距离严格变大或变为不可达。
- 若存在一条最短路 $P$ 不经过 $c$：删去 $c$ 不会删掉 $P$ 的任何点/边，$P$ 仍在图中，最短距离不可能变大（删边只会让距离不减，而 $P$ 保持了原距离），故距离不变。

两个方向合起来即得。$\blacksquare$

于是问题变成：找出所有「存在一对 $a,b$，使 $c$ 位于 $a\to b$ 的每条最短路上」的点 $c$。

### 2.2 一个看似可行但错误的判据

**错误想法**：若存在 $a,b$ 使 $\operatorname{dis}(a,c)+\operatorname{dis}(c,b)=\operatorname{dis}(a,b)$，就认为 $c$ 重要。

**极简反例**：三角形，边 $1-2$ 权 $1$，$2-3$ 权 $1$，$1-3$ 权 $2$。取 $a=1,b=3,c=2$，有 $\operatorname{dis}(1,2)+\operatorname{dis}(2,3)=1+1=2=\operatorname{dis}(1,3)$，但删去 $2$ 后 $1\to3$ 还可以走直边，距离仍是 $2$。所以 $2$ 不是重要城市。

错误原因：$\operatorname{dis}(a,c)+\operatorname{dis}(c,b)=\operatorname{dis}(a,b)$ 只说明 $c$ 在**某一条**最短路上，而引理 1 要求 $c$ 在**每一条**最短路上。

### 2.3 从「每一条」出发：最短路 DAG 上的支配点

固定源点 $a$。把所有满足

$$
\operatorname{dis}(a,u)+w(u,v)=\operatorname{dis}(a,v)
$$

的有向边 $u\to v$ 拿出来，得到一张 DAG（因为边权为正，沿边距离严格增加）。这张图上 $a\to b$ 的每条路径都对应原图的一条最短路，反之每条最短路也都以这种方式出现。

于是「$c$ 在 $a\to b$ 的每条最短路上」就变成「$c$ 支配 $b$」（$c$ 是 DAG 中从 $a$ 到 $b$ 的必经点）。记

$$
\operatorname{dom}[u]=\{\,v:\ v\text{ 在 DAG 中位于从 }a\text{ 到 }u\text{ 的每条路径上}\,\}.
$$

### 2.4 引理：DAG 上的支配集合递推

**引理 2**：按拓扑序（这里按 $\operatorname{dis}(a,\cdot)$ 升序）处理点，则

$$
\operatorname{dom}[a]=\{a\},\qquad
\operatorname{dom}[u]=\{u\}\cup\bigcap_{v\to u\text{ 是 DAG 边}}\operatorname{dom}[v]\quad(u\ne a).
$$

**证明**：设 $u\ne a$ 且 $u$ 从 $a$ 可达。记 $P_u=\{v: v\to u \text{ 是 DAG 边}\}$，它非空（取任意一条 $a\to u$ 最短路的最后一条边）。

先证右端 $\subseteq$ 左端。取 $c\in\bigcap_{v\in P_u}\operatorname{dom}[v]$。任取 DAG 中一条 $a\to u$ 的路径，它的最后一条边是某个 $v\to u$（$v\in P_u$），前缀是 $a\to v$ 的路径。由于 $c$ 支配所有 $a\to v$ 路径，$c$ 在前缀上；又 $c\ne u$（因为 $c\in\operatorname{dom}[v]$ 而 $\operatorname{dom}[v]$ 的元素都在 $u$ 之前，见拓扑序），所以 $c$ 出现在整条 $a\to u$ 路径上。故 $c\in\operatorname{dom}[u]$。另外 $u\in\operatorname{dom}[u]$ 显然。

再证左端 $\subseteq$ 右端。取 $c\in\operatorname{dom}[u]$，$c\ne u$。对任意 $v\in P_u$，任取一条 $a\to v$ 的路径再接上边 $v\to u$，得到一条 $a\to u$ 的路径，它经过 $c$；由于 $c\ne u$，$c$ 必在该前缀路径上。故 $c\in\operatorname{dom}[v]$ 对所有 $v\in P_u$ 成立。$\blacksquare$

**为什么按距离升序就是拓扑序**：DAG 边 $v\to u$ 满足 $\operatorname{dis}(a,v)+w(v,u)=\operatorname{dis}(a,u)$ 且 $w>0$，故 $\operatorname{dis}(a,v)<\operatorname{dis}(a,u)$，前驱一定先被处理。距离相同的点之间不可能有 DAG 边，因此排序的稳定性无关紧要。

### 2.5 整体算法

对每个源点 $a$ 都建一次最短路 DAG 并算一次支配集合，然后：

$$
c\text{ 重要}\iff \exists a,u:\ c\in\operatorname{dom}[u],\ c\ne a,\ c\ne u .
$$

$c\ne a$ 排除源点本身，$c\ne u$ 排除终点本身（题目要求 $a,b$ 都不同于 $c$）。所有点对 $(a,u)$ 都会被枚举到，所以不漏。

**为什么不用「删点后重跑 Floyd」**：对每个 $c$ 重跑一次 Floyd 是 $O(N^4)\approx1.6\times10^9$，在 $N=200$ 时很容易超时；而支配集合的方法总复杂度约为 $O(N^3+NM\cdot N/64)$，$N=200,M=2\times10^4$ 时约 $3\times10^7$ 次字运算，快得多，而且是确定性的（不需要对「最短路条数」取模或哈希）。

**为什么不用「最短路计数」**：最短路条数可以达到指数级（远超 `long long`），直接存会溢出；取模又会引入碰撞风险。支配关系直接回答「是否所有最短路都经过它」，既精确又省事。

### 2.6 边界情况

- 图不连通：不可达的点对 $\operatorname{dis}=\infty$，不存在「最短路」，自然不会贡献答案。算法中 `dis[a][u]>=INF` 的点直接跳过；不可达点的 `dom` 也不会被用到。
- $N=1$：没有任何点对，输出 `No important cities.`。
- 只有一个点的图、只有一条边的图，均按上述逻辑自然处理。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 把「删点后距离变大」等价改写为「该点在**所有**最短路上」（引理 1）。
- **consider easier version / find counterexample** — 只判断「在某条最短路上」会错，三角形反例说明必须要求「每一条」。
- **reduce to easier problem** — 固定源点，把所有最短路收成一张正权 DAG。
- **classic problem solution** — 在 DAG 上按拓扑序递推支配集合 $\operatorname{dom}[u]=\{u\}\cup\bigcap_{\text{pred}}\operatorname{dom}[\text{pred}]$，用 `bitset` 做交集。
- **explore its property** — 对每个源点都做一遍，标记所有非端点的支配点即为答案。

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, m` | `int` | 点数、边数 | 读入 | $\le200$ |
| `dis[i][j]` | `int [205][205]` | 全源最短路（Floyd） | 无边 `INF`，`dis[i][i]=0` | distance |
| `graph[u]` | `vector<pair<int,int>>` | 邻接表，存 `{v, w}` | 读入 | 建边 |
| `ord[]` | `int [205]` | 按 `dis[s][*]` 升序排好的点序 | 每个源点重排 | order |
| `dom[u]` | `bitset<205>` | 源点 $s$ 的最短路 DAG 中，$s\to u$ 所有路径的必经点集合 | `dom[s]={s}` | dominators |
| `important[c]` | `bool [205]` | 城市 $c$ 是否重要 | `false` | 答案标记 |
| `INF` | `constexpr int` | 不可达哨兵 $10^9$ | 常量 | 两两相加仍不溢出 `int` |

### 4.2 算法步骤

1. 读入图，初始化 `dis`：无边为 `INF`，`dis[i][i]=0`，同时建邻接表 `graph`。
2. **Floyd**：`for k,i,j: dis[i][j]=min(dis[i][j], dis[i][k]+dis[k][j])`。
   （`INF+INF=2×10^9<2^{31}-1`，不会溢出。）
3. 对每个源点 `s=0..n-1`：
   1. 把 `0..n-1` 按 `dis[s][x]` 升序排序存入 `ord`；
   2. 清空所有 `dom`，令 `dom[s]` 只有第 `s` 位为 1；
   3. 按 `ord` 遍历 `u`：
      - 若 `u==s` 或 `dis[s][u]>=INF`，跳过；
      - 令临时 `bitset` 全 1；对每条邻边 `(u,v,w)`，若 `dis[s][v]<INF` 且 `dis[s][v]+w==dis[s][u]`，则 `bs &= dom[v]`，并标记「有前驱」；
      - 若有前驱：`bs.set(u)`，`dom[u]=bs`；
      - 对 `bs` 中每个为 1 的 `c`，若 `c!=s && c!=u`，置 `important[c]=true`。
4. 按编号递增输出所有 `important[i]`；若一个都没有，输出 `No important cities.`。

### 4.3 复杂度

- 时间：Floyd $O(N^3)$；每个源点排序 $O(N\log N)$，遍历邻边总计 $O(M)$，每条有效前驱边做一次 $O(N/64)$ 的 `bitset` 交，再花 $O(N)$ 标记；合计 $O\!\left(N^3+NM\cdot\frac{N}{64}+N^3\right)$。$N=200$ 时约 $3\times10^7$ 次字运算。
- 空间：`dis` 为 $O(N^2)$，`dom` 为 $O(N^2/64)$ 个字，邻接表 $O(M)$。

## 5. 示例代码

```cpp
//
// 题目：P1841 [JSOI2007] 重要的城市
// https://www.luogu.com.cn/problem/P1841
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{205};
constexpr int inf{1000000000};

int n,m;
int dis[mxn][mxn];
bool important[mxn];
vector<pair<int,int>>graph[mxn];
int ord[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	fill_n(&dis[0][0],mxn*mxn,inf);
	for(int i=0;i<n;++i)
		dis[i][i]=0;
	for(int i=0,u,v,c;i<m;++i)
	{
		cin>>u>>v>>c;
		--u;--v;
		dis[u][v]=dis[v][u]=c;
		graph[u].push_back({v,c});
		graph[v].push_back({u,c});
	}
	for(int k=0;k<n;++k)
		for(int i=0;i<n;++i)
			for(int j=0;j<n;++j)
				if(dis[i][k]+dis[k][j]<dis[i][j])
					dis[i][j]=dis[i][k]+dis[k][j];

	static bitset<mxn>dom[mxn];
	for(int s=0;s<n;++s)
	{
		for(int i=0;i<n;++i)
			ord[i]=i;
		sort(ord,ord+n,[s](int a,int b){return dis[s][a]<dis[s][b];});
		for(int i=0;i<n;++i)
			dom[i].reset();
		dom[s].set(s);
		for(int t=0;t<n;++t)
		{
			int u=ord[t];
			if(u==s||dis[s][u]>=inf)
				continue;
			bitset<mxn>bs;
			bs.set();
			bool any{false};
			for(auto[v,c]:graph[u])
				if(dis[s][v]<inf&&dis[s][v]+c==dis[s][u])
				{
					bs&=dom[v];
					any=true;
				}
			if(!any)
				continue;
			bs.set(u);
			dom[u]=bs;
			for(int c=0;c<n;++c)
				if(bs[c]&&c!=s&&c!=u)
					important[c]=true;
		}
	}
	int cnt{0};
	for(int i=0;i<n;++i)
		if(important[i])
		{
			if(cnt++)
				cout<<' ';
			cout<<i+1;
		}
	if(!cnt)
		cout<<"No important cities.";
	cout<<'\n';
	return 0;
}
```
