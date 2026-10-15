# P1123 取数游戏 — 完整题解

---

## 1. Re-read the problem

**题意重述**

给定一个 $N\times M$ 的非负整数矩阵（$N,M\le 6$，$T\le 20$），从中选出若干数字，要求任意两个被选中的数字**不相邻**。这里的"相邻"定义为八连通——若一个数字在另一个数字的 $8$ 个相邻格子（上、下、左、右、四个对角）之一，即认为相邻。求选出数字之和的最大值。

**样例理解**

矩阵（第一组数据）：

$$
\begin{matrix}
67 & 75 & 63 & 10 \\
29 & 29 & 92 & 14 \\
21 & 68 & 71 & 56 \\
8 & 67 & 91 & 25
\end{matrix}
$$

最优选法用 $[\cdot]$ 标记：

$$
\begin{matrix}
[67] & 75 & 63 & 10 \\
29 & 29 & [92] & 14 \\
[21] & 68 & 71 & 56 \\
8 & 67 & [91] & 25
\end{matrix}
$$

选中 $67,92,21,91$，和为 $67+92+21+91=271$。验证相邻性：$67$ 的八邻域含 $75,29,29$ 均未被选；$92$ 的八邻域含 $63,10,14,71,56$ 均未被选，其余同理。

**约束条件**

- $1\le N,M\le 6$（即总格子数 $\le 36$）
- $1\le T\le 20$
- $a_{i,j}\le 10^5$
- 非负整数，无负数

**本质抽象**

在八连通网格图上求**最大权独立集**（Maximum-Weight Independent Set, MWIS）。由于网格极小（$\le 36$ 顶点），可用搜索或状压 DP 解决。

---

## 2. Chain-of-thought with full proofs

### 2.1 朴素想法为何不可行

最直接的思路：枚举每个格子"选或不选"，检查是否两两不相邻。$2^{36}\approx 7\times10^{10}$，不可行。

但我们可以利用**八连通约束的局部性**来剪枝：一旦选中某格，其周围 8 个格子全部被封锁。由于网格极小，DFS 配合回溯可以高效地搜索。

### 2.2 DFS 搜索框架

按**先行后列**的顺序逐个格子决策：对于格子 $(r,c)$，要么跳过，要么在未被封锁时选中。

```
dfs(r, c, sum):
	if r == N: 更新答案, return
	if c == M: dfs(r+1, 0, sum); return

	// 1. 跳过当前格
	dfs(r, c+1, sum)

	// 2. 若未被封锁，选中当前格
	if not banned[r][c]:
		封锁 8 邻域中未被封锁的格子
		dfs(r, c+1, sum + a[r][c])
		恢复被封锁的格子
```

**正确性证明**：每个格子只有"选"或"不选"两种状态，且"选"的决定仅在未被封锁时做出。按顺序决策保证了每种合法的取数方案恰好被枚举一次。跳过/选中的顺序不影响正确性。

**剪枝手段**：

1. **搜索顺序**：先跳过再选中。因为跳过始终合法而选中需要条件，先跳过可以更快地到达叶子节点、更新答案上界。
2. **可行性剪枝**：若 `sum + 剩余所有格子之和 <= 当前最优答案`，直接返回。可以预先计算每个格子的**后缀和**（所有排在它后面的格子权值和）。

### 2.3 复杂度分析 — 上界估值

最坏情况下，DFS 会遍历所有"合法的取数方案"。在八连通约束下，独立集规模的上界是多少？

**引理 1**：在八连通的 $N\times M$ 网格中，任意一个合法取数方案至多选中 $\left\lceil\dfrac{NM}{4}\right\rceil$ 个格子。

**证明**：将网格划分为互不相交的 $2\times 2$ 方块（$N,M$ 为偶数时恰好划分，为奇数时边缘格子单独处理）。在每个 $2\times 2$ 方块内，任意两个格子都八连通，因此最多选 1 个。故 $\text{独立集大小}\le \#\text{blocks} = \left\lceil\dfrac{N}{2}\right\rceil\times\left\lceil\dfrac{M}{2}\right\rceil \le \left\lceil\dfrac{NM}{4}\right\rceil$。当 $N=M=6$ 时，最多 $\left\lceil\dfrac{36}{4}\right\rceil = 9$ 个格子。$\square$

**推论**：搜索树深度 $\le 36$，但选中分支最多 $9$ 层，非选中分支最多 $27$ 层。搜索树的宽度的上界是 $\binom{36}{9} \approx 9.4\times 10^7$，但结合邻域封锁和最优性剪枝，实际访问的状态数远小于此，可以在极短时间内通过 $N=M=6$ 的数据。

### 2.4 状压 DP — 更优的确定性解法

由于 $M\le 6$，行状态可以用一个 $M$ 位二进制数表示（第 $j$ 位为 $1$ 表示该行第 $j$ 列被选中）。$2^6=64$ 种状态，非常小。

**状态定义**

$$
dp[i][mask] = \text{考虑前 } i \text{ 行（0-indexed），第 } i \text{ 行的选取状态为 } mask \text{ 时的最大和}
$$

**行内合法性**

一个行状态 $mask$ 合法，当且仅当其中没有相邻的 1（水平方向不相邻）：

$$
mask\ \&\ (mask \ll 1) = 0
$$

**行间相容性**

行 $i$ 的状态 $mask_1$ 与行 $i-1$ 的状态 $mask_2$ 相容，当且仅当它们不产生八连通邻接：

1. **垂直相邻**：$mask_1\ \&\ mask_2 = 0$
2. **左上-右下对角**：$(mask_1 \ll 1)\ \&\ mask_2 = 0$
3. **右上-左下对角**：$(mask_1 \gg 1)\ \&\ mask_2 = 0$

**证明**：八连通中，$(i,j)$ 与 $(i-1,j)$ 垂直相邻，对应 $mask_1$ 的第 $j$ 位与 $mask_2$ 的第 $j$ 位不能同时为 1。$(i,j)$ 与 $(i-1,j-1)$ 对角相邻，对应 $mask_1$ 的第 $j$ 位与 $mask_2$ 的第 $j-1$ 位（即 $mask_2\ll 1$ 的第 $j$ 位）不能同时为 1。$(i,j)$ 与 $(i-1,j+1)$ 同理。$\square$

**转移方程**

$$
dp[i][mask] = \max_{\substack{mask_2\ \text{合法}\\ mask\ \text{与}\ mask_2\ \text{相容}}} \Big( dp[i-1][mask_2] + \text{val}(i, mask) \Big)
$$

其中 $\text{val}(i, mask)$ 是第 $i$ 行在状态 $mask$ 下选中的数字之和。

边界条件：$dp[0][mask] = \text{val}(0, mask)$ 对所有合法 $mask$。

最终答案：$\displaystyle\max_{mask} dp[N-1][mask]$。

**复杂度**：$O(N\cdot 2^M\cdot 2^M) = O(6\times 64\times 64) = O(24576)$，对 $T\le 20$ 组数据完全无压力。

---

## 3. Pattern summary

**题目类型**：网格图最大权独立集（Maximum-Weight Independent Set on Grid Graph）

**核心范式**：

1. **搜索解法**：按顺序逐格决策 + 邻域封锁回溯。适用于约束强、网格小的 MWIS 问题。
2. **状压 DP 解法**：逐行处理，用 $M$ 位比特表示行内选择状态。适用于 $M\le 6$ 的窄网格。

**何时用搜索 vs DP**：

| 标准 | 搜索 | 状压 DP |
|------|------|---------|
| 实现难度 | 低，适合现场快速编写 | 中等，需预处理相容性 |
| $M$ 限制 | 无特殊限制（格子总数 $\le 36$ 均可） | 要求 $M\le 6$（$2^M\le 64$） |
| 运行时间 | 较快，但最坏仍指数 | 严格多项式 $O(N4^M)$ |
| 扩展性 | 易加剪枝、易处理特殊约束 | 约束格式固定 |

**本题推荐**：两种均可，但状压 DP 的复杂度更确定，适合追求满分；DFS 实现更直观，适合快速 AC。

---

## 4. Real solution with notation glossary

### 符号表

| 符号 | 含义 |
|------|------|
| $N, M$ | 矩阵行数、列数 |
| $a_{i,j}$ | 矩阵第 $i$ 行第 $j$ 列的数字 |
| $\text{ban}_{i,j}$ | （DFS 版）格子 $(i,j)$ 是否被封锁 |
| $ans$ | 当前搜索到的最大和 |
| $mask$ | （DP 版）一个 $M$ 位二进制数，表示某行的选取状态 |
| $dp[i][mask]$ | 前 $i$ 行、第 $i$ 行状态为 $mask$ 的最大和 |
| $\text{val}[i][mask]$ | 第 $i$ 行在状态 $mask$ 下的权值和 |
| $\text{ok}[mask]$ | 是否不存在水平相邻的被选格子 |
| $\text{valid}[mask_1][mask_2]$ | 两行状态是否八连通相容 |

### DFS 解法

```
1. 读入 N, M, 矩阵 a[][]
2. 预处理后缀和 suf[r][c] = 从 (r,c) 到末尾的所有格子之和
3. 调用 dfs(0, 0, 0)
4. 输出 ans

dfs(r, c, cur):
   if r == N: ans = max(ans, cur); return
   if c == M: dfs(r+1, 0, cur); return
   // 最优性剪枝：就算后面全取也不可能超过 ans
   if cur + suf[r][c] <= ans: return

   // 不取 (r, c)
   dfs(r, c+1, cur)

   // 取 (r, c)（若未被封锁）
   if not ban[r][c]:
      记录本次封锁的格子列表（只记录之前未封锁的）
      封锁 8 邻域
      dfs(r, c+1, cur + a[r][c])
      恢复封锁
```

注意：只记录**在这次操作之前**未封锁的格子，以便回溯恢复。

### 状压 DP 解法

```
1. 读入 N, M, 矩阵 a[][]
2. 枚举所有 mask (0 .. 2^M-1):
     计算 val[mask]（某行在 mask 下的和）
     计算 ok[mask]（无水平相邻）
3. 枚举所有 mask1, mask2:
     计算 valid[mask1][mask2]（八连通相容）
4. 初始化 dp[0][mask] = val[mask] 对 ok[mask] 为真的 mask
5. for i = 1 to N-1:
     for each mask (ok[mask] 为真):
         dp[i][mask] = max_{mask2: ok[mask2] && valid[mask][mask2]}
                       dp[i-1][mask2] + val[mask]
6. 答案 = max_{mask} dp[N-1][mask]
```

---

## 5. Example code

### 解法一：DFS 回溯

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{11};
int T, n, m, a[mxn][mxn];
bool ban[mxn][mxn];
int suf[mxn][mxn];
int ans;

void dfs(int r, int c, int cur)
{
	if (r == n)
	{
		ans = max(ans, cur);
		return;
	}
	if (c == m)
	{
		dfs(r + 1, 0, cur);
		return;
	}
	// 最优性剪枝：剩余全取也不可能超过 ans
	if (cur + suf[r][c] <= ans)
		return;

	// 不取
	dfs(r, c + 1, cur);

	// 取（若可）
	if (!ban[r][c])
	{
		// 记录本次操作封锁的格子，用于回溯恢复
		vector<pair<int,int>> vec;
		for (int dr{-1}; dr <= 1; ++dr)
		{
			for (int dc{-1}; dc <= 1; ++dc)
			{
				if (dr == 0 && dc == 0)
					continue;
				int nr = r + dr, nc = c + dc;
				if (nr < 0 || nr >= n || nc < 0 || nc >= m)
					continue;
				if (!ban[nr][nc])
				{
					ban[nr][nc] = true;
					vec.push_back({nr, nc});
				}
			}
		}
		dfs(r, c + 1, cur + a[r][c]);
		// 回溯恢复
		for (auto [x, y] : vec)
			ban[x][y] = false;
	}
}

void solve()
{
	cin >> n >> m;
	for (int i{0}; i < n; ++i)
		for (int j{0}; j < m; ++j)
			cin >> a[i][j];

	// 预处理后缀和
	for (int i{n - 1}; i >= 0; --i)
		for (int j{m - 1}; j >= 0; --j)
			suf[i][j] = a[i][j]
				+ (j + 1 < m ? suf[i][j + 1] : 0)
				+ (i + 1 < n ? suf[i + 1][0] : 0)
				- (i + 1 < n && j + 1 < m ? suf[i + 1][j + 1] : 0);

	ans = 0;
	memset(ban, 0, sizeof(ban));
	dfs(0, 0, 0);
	cout << ans << '\n';
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin >> T;
	while (T--)
		solve();
	return 0;
}
```

### 解法二：状压 DP

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{11};
int T, n, m;
int a[mxn][mxn];
int val[1 << 6];
int dp[mxn][1 << 6];
bool ok[1 << 6];
bool valid[1 << 6][1 << 6];

void solve()
{
	cin >> n >> m;
	for (int i{0}; i < n; ++i)
		for (int j{0}; j < m; ++j)
			cin >> a[i][j];

	int S = 1 << m;

	// 预处理行内合法性 & 行权值和
	for (int mask{0}; mask < S; ++mask)
	{
		ok[mask] = (mask & (mask << 1)) == 0;
		val[mask] = 0;
		for (int j{0}; j < m; ++j)
			if (mask >> j & 1)
				val[mask] += a[0][j]; // 先填第一行，后续逐行更新
	}

	// 预处理行间相容性
	for (int m1{0}; m1 < S; ++m1)
		for (int m2{0}; m2 < S; ++m2)
			valid[m1][m2] = (m1 & m2) == 0
				&& ((m1 << 1) & m2) == 0
				&& ((m1 >> 1) & m2) == 0;

	// 初始化第 0 行
	for (int mask{0}; mask < S; ++mask)
	{
		if (ok[mask])
		{
			dp[0][mask] = 0;
			for (int j{0}; j < m; ++j)
				if (mask >> j & 1)
					dp[0][mask] += a[0][j];
		}
		else
			dp[0][mask] = -1e9;
	}

	// DP 转移
	for (int i{1}; i < n; ++i)
	{
		// 计算当前行各 mask 的权值和
		for (int mask{0}; mask < S; ++mask)
		{
			val[mask] = 0;
			for (int j{0}; j < m; ++j)
				if (mask >> j & 1)
					val[mask] += a[i][j];
		}

		for (int mask{0}; mask < S; ++mask)
		{
			if (!ok[mask])
			{
				dp[i][mask] = -1e9;
				continue;
			}
			dp[i][mask] = -1e9;
			for (int pre{0}; pre < S; ++pre)
			{
				if (!ok[pre] || !valid[mask][pre])
					continue;
				dp[i][mask] = max(dp[i][mask], dp[i - 1][pre] + val[mask]);
			}
		}
	}

	int ans = 0;
	for (int mask{0}; mask < S; ++mask)
		ans = max(ans, dp[n - 1][mask]);
	cout << ans << '\n';
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin >> T;
	while (T--)
		solve();
	return 0;
}
```

### 代码要点说明

| 部分 | 说明 |
|------|------|
| DFS 后缀和剪枝 | `suf[r][c]` 预处理从 $(r,c)$ 到末尾的所有格子之和；若 `cur + suf[r][c] <= ans` 则剪枝 |
| DFS 邻域封锁 | 只记录**本次操作之前**未封锁的格子，回溯时只恢复这些格子 |
| 状压 `ok[mask]` | `mask & (mask << 1) == 0` 保证无水平相邻 |
| 状压 `valid[m1][m2]` | 三个条件分别对应**垂直**、**左下-右上对角**、**左上-右下对角**的不相邻 |
| 状压 DP 初值 | 第 0 行的非法状态置为 $-\infty$（$-10^9$），保证不会被转移 |
| `val[mask]` 复用 | 每行独立计算权值和，无需存储 $N\times 2^M$ 的数组 |

---

**总结**：P1123 是一道网格图最大权独立集的经典入门题。$N,M\le 6$ 的极小的数据范围允许我们用 DFS 回溯或状压 DP 通过。DFS 思路直观、代码宜调试；状压 DP 复杂度严格确定、适合 $M$ 固定的场景。掌握这两种解法，就掌握了小规模网格 MWIS 问题的核心范式。
