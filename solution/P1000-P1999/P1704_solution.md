# P1704 寻找最优美做题曲线

## 1. 重新读题

给定未来 $N$ 天（$1 \le N \le 5 \times 10^5$）每天固定能做出的题目数量 $c_1, c_2, \dots, c_N$（$1 \le c_i \le 10^9$），以及 $K$ 个必须刷题的日期 $p_1, p_2, \dots, p_K$（$1 \le K \le N/2$，各 $p_i$ 互不相同，输入不保证有序）。

我们需要从 $\{1, 2, \dots, N\}$ 中选出一个下标严格递增的子序列 $b_1 < b_2 < \dots < b_m$，满足以下两条硬约束：

1. **必选日期约束**：所有给定的 $K$ 个日期都必须被选中，即 $\{p_1, p_2, \dots, p_K\} \subseteq \{b_1, b_2, \dots, b_m\}$；
2. **严格递增约束**：所谓“做题曲线 $(1, c_{b_1}) \to (2, c_{b_2}) \to \dots \to (m, c_{b_m})$ 保持严格单调递增”，等价于被选出日期的做题数量严格递增：
   $$c_{b_1} < c_{b_2} < \dots < c_{b_m}.$$

若不存在任何满足上述两条约束的子序列，输出 `impossible`；否则输出最大可能的长度 $m$。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：必选点把整个序列切成了什么？

先把题面里的几何包装剥掉：横坐标 $1, 2, \dots, m$ 天生严格递增，折线严格上升当且仅当纵坐标序列 $c_{b_1} < c_{b_2} < \dots < c_{b_m}$ 严格递增。因此，原问题就是在序列 $c_1, \dots, c_N$ 中找一条**必须经过指定 $K$ 个下标**的最长严格上升子序列（LIS）。

既然 $K$ 个必选日期的输入顺序是乱的，而子序列下标必须按时间顺序排列，第一步自然是把必选下标排序：
$$1 \le p_1 < p_2 < \dots < p_K \le N.$$

为了统一处理首尾边界（$p_1$ 之前的部分与 $p_K$ 之后的部分），我们引入两个虚拟哨兵端点：
$$p_0 = 0,\quad c_0 = -\infty;\qquad p_{K+1} = N + 1,\quad c_{N+1} = +\infty,$$
并将它们也视为必选点。这样一来，$K+2$ 个必选点把全部下标切成了 $K+1$ 个互不重叠的开区间 $(p_t, p_{t+1})$（$0 \le t \le K$）。

### 2.2 简化版本与可行性判定

先问一个最基本的问题：**什么时候会输出 `impossible`？**

如果连必选点自身都无法构成严格上升序列，即存在某个 $t \in \{0, 1, \dots, K\}$ 使得 $c_{p_t} \ge c_{p_{t+1}}$，由于 $p_t < p_{t+1}$ 必须同时出现在子序列中且先后次序固定，任何包含它们的子序列都在这两点处违反严格递增，必定无解。

反过来，如果必选点自身已经满足 $c_{p_1} < c_{p_2} < \dots < c_{p_K}$，我们哪怕**一天额外的题都不做**，直接只选 $\{p_1, \dots, p_K\}$ 这 $K$ 天，就已经是一个合法解了！

**引理 1（可行性充要条件）**：原问题存在合法做题曲线，当且仅当排序后的必选下标满足 $c_{p_1} < c_{p_2} < \dots < c_{p_K}$。

**证明**：
- **必要性**：设存在合法子序列 $b_1 < b_2 < \dots < b_m$ 包含全部 $p_t$。因 $p_1 < p_2 < \dots < p_K$，它们在 $b$ 中对应的位置下标也严格递增；由 $c_{b_1} < \dots < c_{b_m}$ 的传递性，必有 $c_{p_1} < c_{p_2} < \dots < c_{p_K}$。
- **充分性**：若 $c_{p_1} < c_{p_2} < \dots < c_{p_K}$，直接取子序列 $(b_1, \dots, b_K) = (p_1, \dots, p_K)$，它包含了全部必选点且值严格递增，故合法解存在。$\blacksquare$

### 2.3 碰壁与反例：直接套全局 LIS 为什么错？

**失败尝试 1：在每段 $(p_t, p_{t+1})$ 内直接求普通 LIS。**

举个极简反例：$N = 3, K = 1$，$p_1 = 3$，$c = [10, 20, 5]$。
- 必选点只有 $p_1 = 3$（值 $c_3 = 5$），满足引理 1，有解。
- 若在区间 $(0, 3)$（即前两天 $10, 20$）里直接求 LIS，会选出 $[10, 20]$，再拼上 $c_3 = 5$ 得到 $[10, 20, 5]$，在最后一步违反了 $< c_3$ 的要求！
- 失败原因：区间 $(p_t, p_{t+1})$ 内选出的每一个元素 $c_i$，不仅要彼此严格递增，还必须同时满足**值域上下界** $c_{p_t} < c_i < c_{p_{t+1}}$。

**失败尝试 2：过滤掉不合法值后，用 $O(N^2)$ DP 求每段的 LIS。**

虽然正确，但当 $K=1, p_1=N$ 时，单个区间的长度可达 $N \approx 5 \times 10^5$，$O(N^2)$ 会直接超时。

### 2.4 分段独立性与贪心二分 LIS（附完整证明）

**引理 2（区间解耦定理）**：设引理 1 的条件成立。对于每个开区间 $I_t = (p_t, p_{t+1})$（$0 \le t \le K$），定义过滤序列
$$A_t = \big(c_i \;\big|\; p_t < i < p_{t+1} \text{ 且 } c_{p_t} < c_i < c_{p_{t+1}}\big)$$
（保持原下标递增顺序）。则全局最长合法子序列的长度等于
$$K + \sum_{t=0}^{K} \operatorname{LIS}(A_t),$$
其中 $\operatorname{LIS}(A_t)$ 表示序列 $A_t$ 的最长严格上升子序列长度。

**证明**：
任取一个全局合法子序列 $B$。因为 $\{p_1, \dots, p_K\} \subseteq B$，我们可以按必选点将 $B$ 唯一拆分为 $K$ 个必选点以及落在各开区间 $I_t$ 内的子序列 $B_t = B \cap I_t$（$0 \le t \le K$）。
1. **上界性**：对每个 $t$，由于 $B$ 整体严格递增且 $p_t < i < p_{t+1}$（$\forall i \in B_t$），$B_t$ 中的每个元素 $c_i$ 都必然满足 $c_{p_t} < c_i < c_{p_{t+1}}$，因此 $B_t$ 必定是过滤序列 $A_t$ 的一个严格上升子序列，从而 $|B_t| \le \operatorname{LIS}(A_t)$。累加得 $|B| = K + \sum_{t=0}^K |B_t| \le K + \sum_{t=0}^K \operatorname{LIS}(A_t)$。
2. **可达性**：反之，对每个 $t$ 独立取 $A_t$ 的一个最长严格上升子序列 $B_t^*$（长度 $\operatorname{LIS}(A_t)$）。由 $A_t$ 的定义，$B_t^*$ 中所有元素均大于 $c_{p_t}$ 且小于 $c_{p_{t+1}}$，且内部严格递增。将
   $$B_0^*,\ p_1,\ B_1^*,\ p_2,\ \dots,\ p_K,\ B_K^*$$
   依次拼接，在每一处衔接点都有 $\max(c_{B_t^*}) < c_{p_{t+1}} < \min(c_{B_{t+1}^*})$，故拼接后的完整序列严格递增且包含全部 $K$ 个必选点，长度恰为 $K + \sum_{t=0}^K \operatorname{LIS}(A_t)$。
综上，等号成立。$\blacksquare$

**引理 3（单调队列/二分维护最小末尾求 LIS）**：对任意序列 $A$，维护数组 $d[0 \dots \text{len}-1]$，其中 $d[l]$ 表示遍历到当前位置时，长度为 $l+1$ 的严格上升子序列的最小可能末尾值。每读入一个元素 $x$，在 $d$ 中用二分查找第一个 $\ge x$ 的位置 $\text{pos} = \operatorname{lower\_bound}(d, d+\text{len}, x) - d$，令 $d[\text{pos}] = x$（若 $\text{pos} = \text{len}$ 则 $\text{len}$ 加 $1$）。遍历结束后 $\text{len} = \operatorname{LIS}(A)$。

**证明**：
先证不变量：在任意时刻，$d[0] < d[1] < \dots < d[\text{len}-1]$ 严格递增。若存在 $d[l-1] \ge d[l]$，则取长度为 $l+1$、末尾为 $d[l]$ 的严格上升子序列，其倒数第二个元素 $y < d[l] \le d[l-1]$，而该前缀本身是长度为 $l$ 的严格上升子序列，末尾 $y < d[l-1]$，与 $d[l-1]$ 的最小性矛盾。
再证转移正确性：当新元素 $x$ 到来时，对任何 $l$，若 $d[l-1] < x$，则将 $x$ 接在长度为 $l$、末尾为 $d[l-1]$ 的子序列后可得长度为 $l+1$、末尾为 $x$ 的合法子序列。由 $d$ 的严格单调性，满足 $d[l-1] < x \le d[l]$ 的位置 $\text{pos} = l$ 唯一（或 $\text{pos}=0$ 时 $x \le d[0]$，或 $\text{pos}=\text{len}$ 时 $x > d[\text{len}-1]$）。因此只有 $d[\text{pos}]$ 会被 $x$ 更新为更小值，其余位置的最小末尾不变。由归纳法，$d$ 始终精确维护各长度的最小末尾，最终 $\text{len}$ 即为最长严格上升子序列长度。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 将折线严格单调上升还原为所选下标子序列的 $c$ 值严格递增。
- **sort & add sentries** — 将无序输入的必选下标 $p_i$ 排序，并在首尾添加 $(0, -\infty)$ 与 $(N+1, +\infty)$ 哨兵以统一区间边界。
- **consider easier version** — 先单独检查必选点自身是否满足 $c_{p_1} < \dots < c_{p_K}$，得出有解的充要条件（引理 1）。
- **failed attempt（未过滤值域）** — 在开区间内直接跑 LIS 会选出超出 $(c_{p_t}, c_{p_{t+1}})$ 的元素，导致跨必选点衔接失败。
- **reduce to independent subproblems** — 过滤出各开区间 $(p_t, p_{t+1})$ 内满足 $c_{p_t} < c_i < c_{p_{t+1}}$ 的元素后，各段完全独立（引理 2）。
- **classic problem solution** — 对每个过滤后的子段使用二分查找维护最小末尾数组求 $O(L \log L)$ 的严格 LIS（引理 3）。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, k` | `int` | 总天数 $N$ 与必选天数 $K$ | 标准输入 | 题目维度 |
| `p[i]` | `int [mxn]` | 排序后的必选日期下标（1-based） | `p[0]=0, p[k+1]=n+1` | positions |
| `c[i]` | `i64 [mxn]` | 第 $i$ 天可 AC 的题目数 | `c[0]=-inf, c[n+1]=inf` | count |
| `d[l]` | `i64 [mxn]` | 当前区间内长度为 $l+1$ 的严格上升子序列最小末尾 | 每段重置 `len=0` | LIS tails |
| `ans` | `int` | 最多可刷题的总天数 | 初值 `k` | answer |

### 4.2 算法步骤

1. 读入 $N, K$，读入数组 $p[1 \dots K]$ 与 $c[1 \dots N]$。
2. 对 $p[1 \dots K]$ 升序排序；设置哨兵 `p[0] = 0, p[k+1] = n+1` 以及 `c[0] = -inf, c[n+1] = inf`。
3. 遍历 $t = 0, 1, \dots, K$：若存在 `c[p[t]] >= c[p[t+1]]`，立即输出 `impossible` 并结束。
4. 令 `ans = k`。对每个区间 $t = 0, 1, \dots, K$：
   - 令 `len = 0`；
   - 顺序扫描下标 $i = p[t] + 1 \dots p[t+1] - 1$：若 `c[i] <= c[p[t]]` 或 `c[i] >= c[p[t+1]]` 则跳过；否则在 `d[0 .. len-1]` 中二分查找 `pos = lower_bound(d, d + len, c[i]) - d`，赋值 `d[pos] = c[i]`，若 `pos == len` 则 `++len`；
   - 将 `len` 累加进 `ans`。
5. 输出 `ans`。

### 4.3 复杂度分析

- **时间复杂度**：必选点排序耗时 $O(K \log K)$；每个下标 $i \in \{1, \dots, N\}$ 最多落入一个开区间并执行一次 $O(\log N)$ 的二分查找。总时间复杂度为 $O(N \log N)$。
- **空间复杂度**：仅使用线性全局数组 `p, c, d`，空间复杂度为 $O(N)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1704 寻找最优美做题曲线
// https://www.luogu.com.cn/problem/P1704
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{500005};
constexpr i64 inf{1145141919810LL};

int n,k;
int p[mxn];
i64 c[mxn],d[mxn];

int calc(int l,int r,i64 low,i64 high)
{
	int len=0;
	for(int i=l+1;i<r;++i)
	{
		if(c[i]<=low||c[i]>=high)
			continue;
		int pos=lower_bound(d,d+len,c[i])-d;
		d[pos]=c[i];
		if(pos==len)
			++len;
	}
	return len;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	for(int i=1;i<=k;++i)
		cin>>p[i];
	for(int i=1;i<=n;++i)
		cin>>c[i];

	sort(p+1,p+k+1);
	p[0]=0;
	p[k+1]=n+1;
	c[0]=-inf;
	c[n+1]=inf;

	for(int i=0;i<=k;++i)
		if(c[p[i]]>=c[p[i+1]])
		{
			cout<<"impossible\n";
			return 0;
		}

	int ans=k;
	for(int i=0;i<=k;++i)
		ans+=calc(p[i],p[i+1],c[p[i]],c[p[i+1]]);

	cout<<ans<<'\n';
	return 0;
}
```
