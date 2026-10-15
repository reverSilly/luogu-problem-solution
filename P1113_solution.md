# P1113 [USACO02FEB] 杂务 题解

---

## 1. Re-read the problem

有 $n$ 项杂务，第 $i$ 项耗时为 $t_i$。某些杂务有**前置依赖**：必须在若干前置杂务全部完成后才能开始。题目保证：

- 至少有一项杂务没有前置依赖（标记为杂务 $1$）；
- 杂务 $k\ (k>1)$ 的准备工作只可能在杂务 $1$ 至 $k-1$ 中；
- 不同杂务可以同时进行（工人无限多）；
- 求完成**所有**杂务所需的最短时间。

输入格式：第 $1$ 行 $n$；接下来 $n$ 行每行依次是：序号、耗时 $t_i$、若干个前置杂务编号（以 $0$ 结尾，若无需前置则只有一个 $0$）。

## 2. Chain-of-thought with full proofs

### 2.1 关键观察

由于依赖方向**严格向后**（$k$ 只依赖 $1..k-1$），按编号 $1\to n$ 顺序处理时，每个任务的所有前置任务都已经处理完毕。这消除了显式拓扑排序的必要。

### 2.2 状态定义

设 $dp[i]$ 表示**完成杂务 $i$ 的最早可能时刻**（即杂务 $i$ 本身的结束时间）。

### 2.3 转移方程

若杂务 $i$ 的前置集合为 $\mathrm{pre}(i)$，则：
- 杂务 $i$ 的**最早开始时间** = $\displaystyle\max_{j\in\mathrm{pre}(i)} dp[j]$，即所有前置全部完成的最晚时刻；
- 杂务 $i$ 的**最早结束时间** = 最早开始时间 $+ t_i$。

$$
dp[i] = t_i + \max_{j\in\mathrm{pre}(i)} dp[j]
$$

边界条件：若 $\mathrm{pre}(i)=\varnothing$（即输入中前置列表只有一个 $0$），则 $\max$ 值为 $0$，$dp[i] = t_i$。

### 2.4 正确性证明

**引理 1**（无后效性）：$dp[i]$ 只依赖于编号小于 $i$ 的任务，因此按 $i=1..n$ 顺序递推时，计算 $dp[i]$ 时所有 $dp[j]\ (j<i)$ 均已确定。

**引理 2**（最优子结构）：设 $S_i$ 为完成 $\{1..i\}$ 中所有任务的最小时间。若 $\mathrm{pre}(i)\ne\varnothing$，则 $i$ 必须在所有前置完成后才能开始，故 $i$ 的最早开始时间不早于 $\max_{j\in\mathrm{pre}(i)} dp[j]$；该下界可达（因为工人无限，前置完成后立即开始 $i$）。故 $dp[i]$ 即为完成 $i$ 的最早时刻。

**定理**：完成所有 $n$ 个杂务的最短时间为 $\displaystyle\max_{1\le i\le n} dp[i]$。

**证明**：由于不同任务可并行，全局完成时间由最后结束的任务决定，即所有 $dp[i]$ 的最大值。不存在任何调度能使所有 $dp[i]$ 同时减小，因为每个 $dp[i]$ 已经被递推式紧地下界定。$\square$

### 2.5 复杂度

- 时间：$O(n + \sum |\mathrm{pre}(i)|)$，遍历每个任务及其前置列表各一次。
- 空间：$O(n)$ 存储 $t_i$ 与 $dp[i]$。

## 3. Pattern summary

**DAG 上的 DP / 最长路**。当拓扑序天然给定（依赖只指向更小编号）时，按编号顺序递推即可完成动态规划。等价于求以时间为权的 DAG 最长路（即关键路径），多起点，取所有终点的最大 $dp$ 值。

关键词：DAG DP、关键路径、拓扑序递推。

## 4. Real solution with notation glossary

### 符号表

| 符号 | 含义 |
|------|------|
| $n$ | 杂务总数 |
| $t_i$ | 完成杂务 $i$ 所需时间 |
| $\mathrm{pre}(i)$ | 杂务 $i$ 的前置杂务编号集合 |
| $dp[i]$ | 完成杂务 $i$ 的最早时刻 |
| $\mathrm{ans}$ | 最终答案，$\max dp[i]$ |

### 算法步骤

1. 读入 $n$。
2. 对 $i=1..n$：
   - 读入序号（可忽略）、耗时 $t_i$。
   - 不断读入 $x$ 直到 $x=0$：若 $x\neq 0$，将 $x$ 加入 $\mathrm{pre}(i)$。
3. 对 $i=1..n$：
   - 若 $\mathrm{pre}(i)=\varnothing$：$dp[i]=t_i$；
   - 否则：$dp[i]=t_i + \displaystyle\max_{j\in\mathrm{pre}(i)} dp[j]$。
4. 输出 $\displaystyle\max_{i=1}^n dp[i]$。

## 5. Example code

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64 = long long;

constexpr int mxn{10000 + 5};

int n;
int t[mxn];
int dp[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin >> n;
	for (int i = 1; i <= n; ++i)
	{
		int id; cin >> id; // 序号与 i 一致，可忽略
		cin >> t[i];
		int pre;
		while (cin >> pre, pre)
		{
			dp[i] = max(dp[i], dp[pre]);
		}
		dp[i] += t[i];
	}

	int ans = 0;
	for (int i = 1; i <= n; ++i)
	{
		if (dp[i] > ans) ans = dp[i];
	}
	cout << ans << '\n';

	return 0;
}
```

### 代码说明

- 读入时直接利用 `dp[i]` 作为临时变量收集前置 $dp$ 的最大值，读完前置后再累加 $t_i$。
- 输入中的序号与 $i$ 一致，故无需额外存储，直接弃读。
- 依赖顺序天然保证 $dp[\mathrm{pre}]$ 已计算完毕。
- 最后扫一遍求 $\max$ 输出即可。
