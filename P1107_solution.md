# P1107 [BJWC2008] 雷涛的小猫 — 完整题解

---

## 1. Re-read the problem | 重读题目

> **题意重述**
>
> - $N$ 棵柿子树，每棵高度均为 $H$。
> - 小猫从阳台跳出，可**任意选择一棵树落在其树顶**（高度 $H$）。
> - 此后每次移动有两种选择：
>   1. **沿当前树向下爬 $1$ 单位**：高度从 $h$ -> $h-1$，树不变。
>   2. **跳跃到任意另一棵树**：高度从 $h$ -> $h-\text{Delta}$，树改变（**不能跳回同一棵**）。
> - 到达每一位置时，若该位置有柿子，则可吃掉（**每个位置可能有多个柿子**）。
> - 过程持续到小猫**落到地面**（高度 $0$）为止。
> - 问：最多能吃到多少柿子？

> **数据范围**
>
> | 参数 | 上限 |
> |---|---|
> | $N$（树数） | $2000$ |
> | $H$（树高） | $2000$ |
> | $\text{Delta}$（跳跃下降） | $H$ |
> | $N_i$（每树柿子数） | $5000$ |
> | 输入文件 | $\le 40\text{MB}$ |
>
> 核心矛盾：朴素 DP 状态 $N \times H = 4\times10^6$ 可接受，但朴素转移 $\Theta(N)$ 会导致 $O(N^2 H) \approx 8\times10^9$，不可行。

---

## 2. Chain-of-thought with full proofs | 思考链与完整证明

### Step 1 — 状态定义

令 $\text{dp}[i][h]$ 表示：**小猫从 $(树\;i,\;高度\;h)$ 出发，一直运动到地面，能吃到的最多柿子数**（含当前位置的柿子）。

目标：$\displaystyle\max_{0\le i<N} \text{dp}[i][H]$，因为小猫可选择任意一棵树从顶开始。

### Step 2 — 转移方程

当前位置 $(i, h)$ 吃完柿子后，可选的下一步有且仅有两种：

1. **向下爬** → $(i, h-1)$，获得 $\text{dp}[i][h-1]$。
2. **跳跃** → $(j, h-\text{Delta}),\;j\neq i$，获得 $\displaystyle\max_{j\neq i}\text{dp}[j][h-\text{Delta}]$。

因此：

$$
\text{dp}[i][h] = a[i][h] + \max\bigg(
\text{dp}[i][h-1],\;
\max_{j\neq i}\text{dp}[j][h-\text{Delta}]
\bigg)
$$

其中 $h-1<1$ 或 $h-\text{Delta}<1$ 时对应项视为 $0$（因为已落地，无柿子可吃）。

### Step 3 — 最优子结构证明

> **引理**：若 $\text{dp}[i][h]$ 的取值来自 $\text{dp}[i][h-1]$（爬下），则 $\text{dp}[i][h-1]$ 本身必须是 $(i, h-1)$ 的最优解；若来自 $\text{dp}[j][h-\text{Delta}]$（跳跃），则 $\text{dp}[j][h-\text{Delta}]$ 必须是 $(j, h-\text{Delta})$ 的最优解。

**反证法**：假设 $\text{dp}[i][h]$ 取自 $\text{dp}[i][h-1]$，但存在一个更大的 $\text{dp}'[i][h-1] > \text{dp}[i][h-1]$。那么从 $(i, h)$ 出发，先落到 $(i, h-1)$，然后按照 $\text{dp}'[i][h-1]$ 的路径走，就可得到更大的 $\text{dp}[i][h]$，矛盾。跳跃情形同理。  

因此转移方程满足**最优子结构**，DP 正确。

### Step 4 — 子问题无后效性

计算 $\text{dp}[i][h]$ 时，只需要**更低高度**的状态 $\text{dp}[\cdot][h-1]$ 和 $\text{dp}[\cdot][h-\text{Delta}]$。因此按 $h = 1 \to H$ **增序**计算即可保证所有依赖已就绪。

### Step 5 — 优化：维护前两大值

瓶颈在于 $\max_{j\neq i}\text{dp}[j][h-\text{Delta}]$。若对每个 $i$ 遍历所有 $j$，单层就是 $O(N^2)$。

**观察**：对于同一个高度 $h$，所有 $i$ 查询的集合相同，只是要排除 $i$ 自身。  
**技巧**：对每个高度 $h$ 维护该高度上 $\text{dp}[\cdot][h]$ 的**最大值**和**次大值**，以及最大值所在的树编号。

```
对于高度 cur = h - Delta：
    best[cur]   = max_i dp[i][cur]          (全局最大)
    id[cur]     = 获得 best[cur] 的树编号
    best2[cur]  = 第二大的 dp 值
```

则 $\max_{j\neq i}\text{dp}[j][\text{cur}]$ 可 $O(1)$ 求出：

$$
\max_{j\neq i} =
\begin{cases}
\text{best}[\text{cur}], & \text{if } \text{id}[\text{cur}] \neq i \\
\text{best2}[\text{cur}], & \text{if } \text{id}[\text{cur}] = i
\end{cases}
$$

因为若最大值的树恰好是当前 $i$，则不能用它（跳跃必须换树），退而求其次用次大值。

**正确性**：当 $N=1$ 时，$\text{best2}=0$，跳跃项恒为 $0$，符合「只有一棵树无法跳跃」的实际含义。

### Step 6 — 复杂度分析

| 维度 | 复杂度 |
|---|---|
| 时间 | $O(N\cdot H)$ — 每个状态 $O(1)$ 转移 |
| 空间 | $O(N\cdot H)$ — 两个 $N\times H$ 数组（约 $32\text{MB}$，通常 OJ 可过）|

---

## 3. Pattern summary | 模式总结

| 维度 | 说明 |
|---|---|
| **题型** | 二维 DP + 同类最大值加速 |
| **领域** | 动态规划 / 递推优化 |
| **难度标签** | 普及/提高- |
| **关键特征** | 状态图是分层网格（树 $\times$ 高度），转移位于本层或跳跃层；排除自身的最大值查询 -> **维护前两大值** |
| **典型变体** | 摘花生（无跳跃）、数字三角形（无横向转移）、滑雪（四方向） |
| **适用范围** | 任意「在每个状态需要查询同行/同列/同层除自身外的全局极值」的问题 |

---

## 4. Real solution with notation glossary | 正解与符号表

### 符号约定

| 符号 | 含义 |
|---|---|
| $N$ | 树的棵数 |
| $H$ | 树的高度 |
| $\text{Delta}$ | 跳跃一次下降的高度 |
| $a[i][h]$ | 树 $i$ 在高度 $h$ 的柿子个数 |
| $\text{dp}[i][h]$ | 从 $(i, h)$ 到地面能获得的最大柿子数 |
| $\text{best}[h]$ | $\max_{0\le i<N}\text{dp}[i][h]$ |
| $\text{id}[h]$ | 取得 $\text{best}[h]$ 的树编号 |
| $\text{best2}[h]$ | $\text{dp}[\cdot][h]$ 的次大值 |

### 算法流程

```
1. 读入 N, H, Delta
2. 读入每棵树的柿子位置，填充 a[i][h]
3. 按 h = 1 .. H 递增计算：
   for i in 0..N-1:
       val = dp[i][h-1]                // 爬下
       if h-Delta >= 1:
           cur = h - Delta
           cand = (id[cur]==i) ? best2[cur] : best[cur]   // 跳跃
           val = max(val, cand)
       dp[i][h] = a[i][h] + val
   计算 best[h], id[h], best2[h]
4. 输出 max_i dp[i][H]
```

### 正确性说明

- **爬下分支**保证在同树连续向下移动时不会丢失最优解。
- **跳跃分支**通过前两大值 $O(1)$ 获得「排除自身的全局最优」，保证「跳到另一棵树」并可继续向下。
- **分高度递推**保证子问题依赖全部已计算，无后效性。
- **答案取 $\max_i \text{dp}[i][H]$** 等价于「选择最优的起始树」。

---

## 5. Example code | 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
constexpr int mxn{2005};

int N, H, Delta;
int a[mxn][mxn];
int dp[mxn][mxn];
int best[mxn];			// best[h] = max_i dp[i][h]
int id[mxn];			// id[h]   = argmax_i dp[i][h]
int best2[mxn];			// best2[h]= second max of dp[*][h]

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin >> N >> H >> Delta;
	for (int i = 0; i < N; ++i)
	{
		int k;
		cin >> k;
		while (k--)
		{
			int x;
			cin >> x;
			++a[i][x];
		}
	}

	for (int h = 1; h <= H; ++h)
	{
		int from = h - Delta;

		for (int i = 0; i < N; ++i)
		{
			int val = dp[i][h - 1];				// 沿树向下 1

			if (from >= 1)
			{
				int cand = (id[from] == i) ? best2[from] : best[from];
				if (cand > val)
					val = cand;				// 跳跃到另一棵树
			}

			dp[i][h] = a[i][h] + val;
		}

		// 维护第 h 高度的前两大值
		int m1 = 0, m1_id = -1, m2 = 0;
		for (int i = 0; i < N; ++i)
		{
			if (dp[i][h] > m1)
			{
				m2 = m1;
				m1 = dp[i][h];
				m1_id = i;
			}
			else if (dp[i][h] > m2)
			{
				m2 = dp[i][h];
			}
		}
		best[h] = m1;
		id[h] = m1_id;
		best2[h] = m2;
	}

	int ans = 0;
	for (int i = 0; i < N; ++i)
	{
		if (dp[i][H] > ans)
			ans = dp[i][H];
	}
	cout << ans << '\n';
	return 0;
}
```

### 代码要点

- **输入加速**：`cin.tie(nullptr); cout.tie(nullptr); ios::sync_with_stdio(false);` 顺序固定。
- **数组维度**：$N, H \le 2000$，`mxn = 2005` 留有余量。
- **柿子计数**：`++a[i][x]` 累计同一高度的多个柿子。
- **递推方向**：`h = 1` 向上至 `H`，保证 `dp[i][h-1]` 和 `best[h-Delta]` 均已算好。
- **前两大值维护**：每层计算完后 $O(N)$ 扫描，总 $O(NH)$，不额外增加复杂度。
- **答案**：$\max_i \text{dp}[i][H]$，对应小猫从任意树顶开始的最优结果。

---
