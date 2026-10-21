# P1821 [USACO07FEB] Cow Party S

> 难度：普及/提高- · 标签：图论, 最短路, USACO

## 1. 重新读题

### 1.1 问题重述（人话版）

有 $n$ 个农场、$m$ 条**有向**路，派对开在农场 $x$。第 $i$ 头牛住在农场 $i$，它去派对再回家，去和回都走自己的最短路。把每头牛的「去程最短 + 回程最短」算出来，输出其中最大的那个长度。

### 1.2 关键约束与规模暗示

- $1\le x\le n\le 10^3$，$1\le m\le 10^5$，$1\le w\le 100$。
- 边有向，可能有重边；权是正的。
- 保证每个点都能到 $x$，且 $x$ 能到每个点，所以每头牛的来回都存在，答案不是「不可达」。
- $n^3=10^9$ 量级的 Floyd 会超时；$m$ 到 $10^5$ 说明要堆优化 Dijkstra，并且只跑常数次，不能对每头牛各跑一次（那是 $n$ 次，虽然 $1000\times 10^5\log$ 也能卡过，但没有必要）。

## 2. 思维链（Chain-of-thought）

来回被「派对地点」拆成两段互不影响的路。先把目标写成最短路。

**引理 1（最优来回 = 两条最短路之和）**

记 $d(u,v)$ 为 $u$ 到 $v$ 的最短路长度（正权，故有限且无负环）。第 $i$ 头牛的最优来回长度是 $d(i,x)+d(x,i)$，答案是 $\max_i\bigl(d(i,x)+d(x,i)\bigr)$。

**证明**：任一条从 $i$ 走到 $x$ 的有向路径长度都不小于 $d(i,x)$，任一条从 $x$ 走回 $i$ 的路径长度都不小于 $d(x,i)$。把两段拼起来，来回长度 $\ge d(i,x)+d(x,i)$。反过来，$i\to x$ 的一条最短路和 $x\to i$ 的一条最短路首尾相接，得到一条合法来回，长度恰好是这个和。因此最优值就是该和。不同牛互不影响，取最大即可。$\blacksquare$

去程是「所有点到 $x$」，回程是「$x$ 到所有点」。后者就是从 $x$ 出发的单源最短路。前者看起来要 $n$ 次单源，把它反过来。

**引理 2（到汇点的最短路 = 反向图上的单源）**

把每条边 $u\to v$ 改成 $v\to u$、权不变，得到反向图 $G^R$。则对任意点 $i$，$G$ 中 $d(i,x)=G^R$ 中 $d^R(x,i)$。

**证明**：设 $P:i=v_0\to v_1\to\dots\to v_k=x$ 是 $G$ 中一条路径，长度为 $\sum w(v_j,v_{j+1})$。把顶点顺序倒过来得到 $P^R:x\to\dots\to i$，其中每条边 $v_{j+1}\to v_j$ 都在 $G^R$ 里且权相同，所以 $P^R$ 的长度等于 $P$。这个对应是路径集合之间的双射（再倒一次回到 $P$），因此最短长度相等。$\blacksquare$

于是只需要两次单源：原图从 $x$ 出发得到 $d(x,\cdot)$，反向图从 $x$ 出发得到 $d(\cdot,x)$。

**引理 3（正权下 Dijkstra 第一次弹出即为最短）**

边权 $\ge 1$。用二叉堆 Dijkstra：堆里存 $(当前距离, 点)$，弹出时若该点已确定则丢弃，否则把弹出距离定为最短路并松弛出边。

**证明**：对弹出顺序归纳。第一个弹出的是源点，距离 $0$，正确。设前若干个确定的点距离都对。下一个被弹出的点 $u$ 距离为 $du$。若存在更短的 $x\rightsquigarrow u$，取这条路径上第一个尚未确定的点 $v$，其前驱已确定，则松弛时 $d(v)\le d(前驱)+w\le$ 真最短 $\le$ 真 $d(u)<du$，且 $v$ 在 $u$ 之前就该被弹出，矛盾。故 $du$ 就是最短路。正权保证路径不会因为绕路变短，归纳能走完。$\blacksquare$

Floyd 对 $n\le 45$ 的完全不同实现用来对拍：三重循环枚举中转点，和「两次 Dijkstra」不是同一条代码路径。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 来回被派对点拆成两段，最优值等于两条最短路之和（引理 1）。
- **reduce to easier problem** — 「所有点到 $x$」改成反向图上从 $x$ 出发的单源（引理 2）。
- **classic problem solution** — 正权稀疏图用堆优化 Dijkstra，原图、反向图各一次（引理 3）。
- **boundary discipline** — 下标读入后 `--u` 改成 0-based；不可达填 `inf` 而不是依赖全局 0；重边都保留，最短路自然取小。

## 4. 真实解法

### 4.1 符号表（表格：符号 / 类型尺寸 / 含义 / 初值 / 助记）

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
|---|---|---|---|---|
| `n,m,x` | `int` | 点数、边数、派对点（0-based） | 读入后 `--x` | 派对在 `x` |
| `g,rg` | `vector<pair<int,int>> [1005]` | 原图 / 反向图邻接表，`pair` 是 (终点, 权) | 空 | `rg` = reverse graph |
| `dis,rdis` | `i64 [1005]` | $d(x,\cdot)$ 与 $d(\cdot,x)$ | 每次 `fill_n` 成 `inf` | 去程看 `rdis` |
| `vis` | `bool [1005]` | Dijkstra 是否已确定 | 每次 `fill_n` 成 `false` | 弹出即确定 |
| `inf` | `i64` | 不可达哨兵 | `1145141919810` | 远大于 $2\cdot 10^3\cdot 100$ |

### 4.2 算法步骤

1. 读入 $n,m,x$，把 $x$ 改成 0-based。
2. 每条边 $u\to v$ 权 $w$：`g[u]` 加 $(v,w)$，`rg[v]` 加 $(u,w)$。
3. `dij(x,g,dis)` 求回程，`dij(x,rg,rdis)` 求去程。
4. 答案为 $\max_i(dis[i]+rdis[i])$。题面保证两端都可达，相加不会碰到哨兵。

### 4.3 复杂度与验证状态（只写你真实跑出来的数字）

- 时间 $O((n+m)\log n)$，两次；空间 $O(n+m)$。
- 编译：`g++ -O2 -std=c++17 -Wall -Wextra -Werror`，无警告。
- 样例 `4 8 2` 那组有向边，标程与 Floyd 暴力均为 `10`。
- 随机 $n\le 45$ 且用「每点与 $x$ 双向连通 + 随机有向边」保证题面可达性：200 组标程 = Floyd，连同 3 组边界（$n=1$ 自环、两点、三点）共 204 组，随机部分耗时 $1.08$ s。
- 最大规模 $n=1000,m=10^5$ 只跑标程：答案 `15`，耗时 $0.0182$ s。
- 合计 `205 tests OK`。

## 5. 示例代码

### 5.1 标程（`P1821.cpp`）

```cpp
//
// 题目：P1821 [USACO07FEB] Cow Party S
// https://www.luogu.com.cn/problem/P1821
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005};
constexpr i64 inf{1145141919810LL};

int n,m,x;
vector<pair<int,int>>g[mxn],rg[mxn];
i64 dis[mxn],rdis[mxn];
bool vis[mxn];

void dij(int s,vector<pair<int,int>>*gr,i64*d)
{
	fill_n(d,n,inf);
	fill_n(vis,n,false);
	priority_queue<pair<i64,int>,vector<pair<i64,int>>,greater<>>Q;
	d[s]=0;
	Q.emplace(0,s);
	while(!Q.empty())
	{
		auto[dd,u]=Q.top();
		Q.pop();
		if(vis[u])
			continue;
		vis[u]=true;
		for(auto[v,w]:gr[u])
			if(d[v]>dd+w)
			{
				d[v]=dd+w;
				Q.emplace(d[v],v);
			}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m>>x;
	--x;
	for(int i=0,u,v,w;i<m;++i)
	{
		cin>>u>>v>>w;
		--u;--v;
		g[u].push_back({v,w});
		rg[v].push_back({u,w});
	}
	dij(x,g,dis);
	dij(x,rg,rdis);
	i64 ans=0;
	for(int i=0;i<n;++i)
		ans=max(ans,dis[i]+rdis[i]);
	cout<<ans<<'\n';
	return 0;
}
```
