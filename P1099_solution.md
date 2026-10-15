# P1099 [NOIP 2007 提高组] 树网的核 — 完整题解

---

## 一、重读问题

给一棵 $n$ 个结点的**无根树**（无圈连通无向图），每条边有正整数长度。定义：

- $d(u,v)$：结点 $u,v$ 之间唯一简单路径的长度和。
- **直径**：树上最长路径。可能有多个，但中心唯一（可能在边的内部）。
- 对于一条路径 $F$，偏心距 $\mathrm{ECC}(F)=\max_{v\in V}\min_{u\in F}d(v,u)$，即距离 $F$ 最远的结点到 $F$ 的距离。
- **核**：某**直径上的一段路径**，长度 $\le s$（端点必须是结点），使得偏心距最小。最小偏心距唯一。

任务：给定 $n,s$ 和边长，输出最小偏心距。

约束：$n\le300,\;s\le10^3,\;w\le10^3$。

---

## 二、发现与推理（含完整证明）

### 2.1 树的直径

> **定义** 直径是树中最长的简单路径。设其长度为 $D$。

用两次 BFS/DFS 或全源最短路可找到直径：任选一点 $p$，找离 $p$ 最远的点 $a$；再从 $a$ 找离 $a$ 最远的点 $b$，则 $a\to b$ 是一条直径。

**约定**：以下固定一条直径 $a\to b$，记 $D=d(a,b)$。

### 2.2 核只考虑一条直径就够了

> **引理 1** 存在一个最小偏心距的核，它是某条直径上的子路径。事实上，只需在任意一条固定的直径上搜索即可。

**证明概要**（常用方法）：设 $F^*$ 是一个最优核，位于直径 $P$ 上。取另一条直径 $Q$。可以证明，将 $F^*$ 的端点沿 $P$ 向 $Q$ "投影"到 $Q$ 上，得到的路径 $F'$ 长度不增加、偏心距不增加。因此存在最优核在 $Q$ 上。由于所有直径的几何中心重合，具体选哪条直径不影响最终结果。$\square$

实际算法中，**任意找一条直径**，在其上枚举子段即可。

### 2.3 投影与到直径的距离

对于任意结点 $v$，考虑它在直径 $a\to b$ 上的**最近点** $P_v$（称为投影）。

> **引理 2** 对任意结点 $v$，
> $$d(v,P_v)=\frac{d(a,v)+d(v,b)-D}{2}$$
> 记 $\mathrm{distToDiam}[v]=d(v,P_v)$。并且 $P_v$ 到 $a$ 的距离为
> $$\mathrm{projPos}[v]=d(a,v)-\mathrm{distToDiam}[v]$$

**证明**：在树中，路径 $a\to v$ 与 $v\to b$ 的交集一直延伸到 $v$；它们与直径 $a\to b$ 分别相交于点 $x$ 和 $y$。由于树无环，$x$ 到 $y$ 这一段是直径的一部分，且 $P_v$ 位于 $x$ 与 $y$ 之间（实际上是 $x$ 与 $y$ 中离 $v$ 更近的端点，但继续推导可知它们重合）。

更直接的代数推导：将 $a\to b$ 走通，有
$$d(a,v)+d(v,b)=d(a,P_v)+2d(P_v,v)+d(P_v,b)=D+2d(P_v,v)$$
移项即得。$\square$

**注意**：$d(a,v)+d(v,b)-D$ 一定是偶数，因为 $2d(P_v,v)$ 是偶数（$d(P_v,v)$ 是整数——树中路径到路径的最短距离必定为整数）。所以整数除法安全。

### 2.4 结点到核子段的距离

设直径上的结点按顺序为 $\mathrm{diam}[0..k-1]$（从 $a$ 到 $b$），记 $\mathrm{pref}[i]=d(a,\mathrm{diam}[i])$。

对核子段 $F=[l,r]$（端点为 $\mathrm{diam}[l],\mathrm{diam}[r]$），其长度为 $\mathrm{pref}[r]-\mathrm{pref}[l]$。考察任意结点 $v$ 到 $F$ 的距离：

- 若 $v$ 的投影 $P_v$ 落在 $\mathrm{diam}[l]$ 与 $\mathrm{diam}[r]$ **之间**（即 $\mathrm{pref}[l]\le\mathrm{projPos}[v]\le\mathrm{pref}[r]$），则 $d(v,F)=\mathrm{distToDiam}[v]$；
- 若 $P_v$ 在左侧（$\mathrm{projPos}[v]<\mathrm{pref}[l]$），则最近点是 $\mathrm{diam}[l]$，距离为 $\mathrm{distToDiam}[v]+\mathrm{pref}[l]-\mathrm{projPos}[v]$；
- 若 $P_v$ 在右侧（$\mathrm{projPos}[v]>\mathrm{pref}[r]$），则最近点是 $\mathrm{diam}[r]$，距离为 $\mathrm{distToDiam}[v]+\mathrm{projPos}[v]-\mathrm{pref}[r]$。

合并为统一公式：
$$d(v,F)=\mathrm{distToDiam}[v]+\max(0,\;\mathrm{pref}[l]-\mathrm{projPos}[v],\;\mathrm{projPos}[v]-\mathrm{pref}[r])$$

### 2.5 偏心距的计算

给定子段 $F=[l,r]$，
$$\mathrm{ECC}(F)=\max_{v\in V}d(v,F)=\max_{v}\Big(\mathrm{distToDiam}[v]+\max(0,\;\mathrm{pref}[l]-\mathrm{projPos}[v],\;\mathrm{projPos}[v]-\mathrm{pref}[r])\Big)$$

### 2.6 算法策略

由于 $n\le300$，$k\le n$，最直接的 $O(k^2n)$ 枚举即可：

1. **Floyd** 计算全源最短路 $d[][]=O(n^3)$。
2. 找到直径端点 $a,b$ 及长度 $D$。
3. 提取直径上的有序结点序列 $\mathrm{diam}[0..k-1]$。
4. 预处理 $\mathrm{distToDiam}[v],\;\mathrm{projPos}[v]$。
5. 枚举所有子段 $[l,r]$（$\mathrm{pref}[r]-\mathrm{pref}[l]\le s$），对每个子段计算 $\mathrm{ECC}$ 并更新答案。

总复杂度 $O(n^3+k^2n)=O(n^3)$，$n=300$ 时约 $27$M 次操作，轻松通过。

---

## 三、模式总结

| 模式 | 说明 |
|------|------|
| **投影到直径** | 树中任意点到直径的最短距离可通过 $d(a,v)+d(v,b)-D$ 的一半求得 |
| **核 = 直径上的子段** | 最优核必在某直径上，只需在一条固定的直径上搜索 |
| **偏心距的三部分** | 左尾巴（$a\to l$）、右尾巴（$r\to b$）、中间（直径外的分支）——可由统一公式计算 |
| **小范围暴力** | $n\le300$ 时 Floyd+二重枚举 $O(n^3)$ 足够 |
| **连续位置处理** | 投影位置是沿直径的连续坐标，不限于结点位置 |

**思考顺序**：树结构 → 找直径 → 投影到直径 → 枚举直径上的子段 → 对每个子段用公式算偏心距 → 取最小。

---

## 四、正解与符号说明

### 算法流程

```
1. 读入 n, s，用 Floyd 计算全源最短路 dist[][]
2. 找直径端点 a,b: max(dist[i][j])
3. 提取直径结点 diam[]: dist[a][i] + dist[i][b] == D
   按 dist[a][i] 排序
4. 计算 pref[i] = dist[a][diam[i]]
5. 对每个结点 v:
     distToDiam[v] = (dist[a][v] + dist[v][b] - D) / 2
     projPos[v] = dist[a][v] - distToDiam[v]
6. ans = INF
   for l = 0..k-1:
     for r = l..k-1:
       if pref[r] - pref[l] > s: continue
       ecc = 0
       for v = 0..n-1:
         cur = distToDiam[v] + max(0, pref[l]-projPos[v], projPos[v]-pref[r])
         ecc = max(ecc, cur)
       ans = min(ans, ecc)
7. 输出 ans
```

### 符号表

| 符号 | 含义 |
|------|------|
| $n$ | 结点数 |
| $s$ | 核的长度上界 |
| $d_{ij}$ | 结点 $i,j$ 间最短路长度（Floyd 后） |
| $D$ | 直径长度 |
| $a,b$ | 一条直径的两个端点 |
| $\mathrm{diam}[i]$ | 直径上第 $i$ 个结点（$a$ 到 $b$ 方向） |
| $k$ | 直径上的结点数 |
| $\mathrm{pref}[i]$ | $d(a,\mathrm{diam}[i])$ |
| $\mathrm{distToDiam}[v]$ | $v$ 到直径的最短距离 |
| $\mathrm{projPos}[v]$ | $v$ 在直径上的投影位置（距 $a$ 的距离） |
| $[l,r]$ | 核子段在 $\mathrm{diam}$ 中的左右下标 |
| $\mathrm{ECC}(F)$ | 路径 $F$ 的偏心距 |
| $\mathrm{ans}$ | 最小偏心距 |

### 边界情况

- $s$ 很大（$\ge D$）：核可取整条直径，偏心距为所有结点到直径的最大距离，即 $\max_v\mathrm{distToDiam}[v]$。
- $s=0$：核退化为单个结点，偏心距为 $\max_v d(v,x)$（$x$ 是直径上某结点）。我们的枚举会覆盖 $l=r$ 的情况。
- 所有边权为正，$n\ge2$，直径上至少有两个结点。
- 若 $k=1$（退化情况，实际不会发生，因为 $n\ge2$ 且边权为正），需特判。

---

## 五、示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{305};
constexpr int inf{114514191};

int d[mxn][mxn];
int n, s;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin >> n >> s;
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			if (i != j)
				d[i][j] = inf;

	for (int i = 1; i < n; ++i)
	{
		int u, v, w;
		cin >> u >> v >> w;
		--u; --v;
		d[u][v] = d[v][u] = w;
	}

	// Floyd
	for (int k = 0; k < n; ++k)
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				if (d[i][k] + d[k][j] < d[i][j])
					d[i][j] = d[i][k] + d[k][j];

	// 找直径端点
	int D = 0, a = 0, b = 0;
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			if (d[i][j] > D)
			{
				D = d[i][j];
				a = i; b = j;
			}

	// 提取直径上的结点
	vector<int> diam;
	for (int i = 0; i < n; ++i)
		if (d[a][i] + d[i][b] == D)
			diam.push_back(i);

	sort(diam.begin(), diam.end(), [&](int x, int y)
	{
		return d[a][x] < d[a][y];
	});

	int k = diam.size();
	vector<int> pref(k);
	for (int i = 0; i < k; ++i)
		pref[i] = d[a][diam[i]];

	// 每个结点到直径的距离 & 投影位置
	vector<int> distToDiam(n), projPos(n);
	for (int v = 0; v < n; ++v)
	{
		distToDiam[v] = (d[a][v] + d[v][b] - D) / 2;
		projPos[v] = d[a][v] - distToDiam[v];
	}

	int ans = inf;
	for (int l = 0; l < k; ++l)
	{
		for (int r = l; r < k; ++r)
		{
			if (pref[r] - pref[l] > s)
				continue;
			int ecc = 0;
			int L = pref[l], R = pref[r];
			for (int v = 0; v < n; ++v)
			{
				int cur = distToDiam[v] + max({0, L - projPos[v], projPos[v] - R});
				if (cur > ecc)
					ecc = cur;
			}
			if (ecc < ans)
				ans = ecc;
		}
	}

	cout << ans << '\n';
	return 0;
}
```
