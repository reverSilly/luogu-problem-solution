# P1725 琪露诺

## 1. 重新读题

小河由编号为 $0, 1, \dots, N$（$N \le 2 \times 10^5$）的格子排成一列。起点在格子 $0$（$A_0 = 0$），每次从格子 $i$ 可以跳到区间 $[i + L, i + R]$（$1 \le L \le R \le N$）内的任意格子 $j$，并获得 $A_j$（$-10^3 \le A_j \le 10^3$）的冰冻指数。只要下一步位置严格大于 $N$ 即视为到达对岸。求到达对岸时可获得的最大冰冻指数总和。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：状态转移与滑动窗口最大值

设 $dp[i]$ 表示到达格子 $i$ 时能获得的最大冰冻指数（若格子 $i$ 不可达，令 $dp[i] = -\infty$）。
因为从 $j$ 能一步跳到 $i$ 当且仅当 $j + L \le i \le j + R$，等价于前驱 $j$ 落在区间 $[i - R, i - L]$ 内：
$$dp[i] = A_i + \max_{\max(0, i-R) \le j \le i-L} dp[j] \qquad (L \le i \le N).$$
而“下一步位置 $> N$ 即可到达对岸”等价于最后一步停留在满足 $i + R > N$（即 $i \in [N - R + 1, N]$）的任意格子 $i$。

### 2.2 单调队列优化（附完整证明）

**引理 1（滑动窗口单调队列正确性与 $\Theta(N)$ 复杂度）**：随着 $i$ 从 $L$ 递增到 $N$，转移前驱区间 $[i - R, i - L]$ 的左右端点每次均向右平移 $1$ 格。维护一个下标递增、对应 $dp$ 值单调递减的双端队列 $Q$：
1. 新元素 $p = i - L$ 入队前，从队尾弹出所有满足 $dp[q] \le dp[p]$ 的下标 $q$，再将 $p$ 压入队尾；
2. 查询前，从队首弹出所有满足 $q < i - R$ 的过期下标；
3. 此时队首 $Q[\text{hd}]$ 的 $dp$ 值即等于 $\max_{j \in [\max(0, i-R), i-L]} dp[j]$。

**证明**：
- **冗余淘汰性**：对任意两个下标 $q < p \le i - L$，若 $dp[q] \le dp[p]$，则由于 $q$ 比 $p$ 更早滑出左边界 $i - R$ 且值不大于 $p$，在此后任何包含 $q$ 的窗口中 $p$ 必然也在窗口内且不劣于 $q$，故 $q$ 永远不可能成为独自最优解，弹出 $q$ 不影响后续任何查询的最大值。
- **单调性**：淘汰后队列中下标严格递增、$dp$ 值严格递减；弹出队首过期下标后，队首元素必在窗口 $[\max(0, i-R), i-L]$ 内且具有最大 $dp$ 值。
- **线性复杂度**：每个下标 $p \in [0, N - L]$ 恰好入队一次、最多出队一次，总操作次数不超过 $2N$，时间复杂度严格为 $\Theta(N)$。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 将正向跳跃范围 $i \to [i+L, i+R]$ 反转为前驱查询区间 $[i-R, i-L]$，终态集合为 $[N-R+1, N]$。
- **classic problem solution** — 识别出固定宽度平移窗口求最值结构，用单调双端队列在 $\Theta(N)$ 时间内优化 DP（引理 1）。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, l, r` | `int` | 格子上限 $N$ 与步长区间 $[L, R]$ | 标准输入 | 维度 |
| `a[i]` | `int [mxn]` | 格子 $i$ 的冰冻指数 | 标准输入 | array |
| `dp[i]` | `int [mxn]` | 到达格子 $i$ 的最大冰冻指数 | `dp[0]=0`，其余 `-inf` | DP table |
| `que` | `int [mxn]` | 单调队列（存下标），配指针 `hd, tl` | 空 | monotonic queue |

### 4.2 算法步骤

1. 读入 $N, L, R$ 及 $A[0 \dots N]$。
2. 初始化 `dp[0] = 0`，`dp[1 .. N] = -inf`，单调队列 `hd = 0, tl = 0`。
3. 枚举 $i = L \dots N$：
   - 令新进入窗口的下标 `p = i - l`；若 `dp[p] > -inf`，当队列非空且 `dp[que[tl-1]] <= dp[p]` 时 `--tl`，随后将 `p` 压入 `que[tl++]`；
   - 当队列非空且 `que[hd] < i - r` 时 `++hd`；
   - 若队列非空，更新 `dp[i] = dp[que[hd]] + a[i]`。
4. 在 $i \in [N - R + 1, N]$ 中取 `dp[i]` 的最大值并输出。

### 4.3 复杂度分析

- **时间复杂度**：每个状态入队、出队至多一次，时间复杂度为 $\Theta(N)$。
- **空间复杂度**：数组 `a, dp, que` 长度均为 $N+5$，空间复杂度为 $\Theta(N)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1725 琪露诺
// https://www.luogu.com.cn/problem/P1725
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{200005};
constexpr int inf{1145141919};

int n,l,r;
int a[mxn],dp[mxn],que[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>l>>r;
	for(int i=0;i<=n;++i)
		cin>>a[i];

	fill_n(dp+1,n,-inf);
	dp[0]=0;
	int hd=0,tl=0;
	for(int i=l;i<=n;++i)
	{
		int p=i-l;
		if(dp[p]>-inf)
		{
			while(hd<tl&&dp[que[tl-1]]<=dp[p])
				--tl;
			que[tl++]=p;
		}
		while(hd<tl&&que[hd]<i-r)
			++hd;
		if(hd<tl)
			dp[i]=dp[que[hd]]+a[i];
	}

	int ans=-inf;
	for(int i=n-r+1;i<=n;++i)
		ans=max(ans,dp[i]);
	cout<<ans<<'\n';
	return 0;
}
```
