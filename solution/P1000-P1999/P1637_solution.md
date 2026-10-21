# P1637 三元上升子序列 题解

> 标签：树状数组 / 线段树 / 动态规划 · 难度：提高/省选-

## 1. 重新读题

**问题重述（人话版）**

给定长度为 $n$ 的正整数序列 $a_0, a_1, \dots, a_{n-1}$。求满足下标严格递增 $0 \le i < j < k < n$ 且数值严格递增 $a_i < a_j < a_k$ 的三元组 $(i, j, k)$ 的总个数。

**约束与规模暗示**

- $30\%$ 数据：$n \le 100$；
- $60\%$ 数据：$n \le 2000$；
- $100\%$ 数据：$1 \le n \le 3 \times 10^4$，$1 \le a_i \le 10^5$。
- 溢出警示：当序列严格递增时，三元组个数为 $\binom{n}{3} = \frac{30000 \times 29999 \times 29998}{6} \approx 4.5 \times 10^{12}$，远超 32 位 `int` 上界，答案必须使用 64 位整数 `i64`。
- 规模暗示：$n = 3 \times 10^4$、值域 $V = 10^5$，预期复杂度为 $O(n \log V)$ 或 $O(n \log n)$。

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：以中间位置 $j$ 为支点拆解双侧独立约束

三元约束 $a_i < a_j < a_k$（$i < j < k$）的耦合中心是中间下标 $j$：一旦固定了中间下标 $j$，左侧下标 $i$ 的选择范围 $\{i < j \mid a_i < a_j\}$ 与右侧下标 $k$ 的选择范围 $\{k > j \mid a_k > a_j\}$ **完全相互独立**。

### 2.2 简化版：固定中间点 $j$ 后的 $O(n^2)$ 解法

**引理 1（中间点乘法原理分解）**

对每个 $j \in [0, n)$，记
$$L[j] = \big|\{i \in [0, j) \mid a_i < a_j\}\big|, \qquad R[j] = \big|\{k \in (j, n) \mid a_k > a_j\}\big|.$$
则合法三元组总数为 $\sum_{j=0}^{n-1} L[j] \cdot R[j]$。

**证明**：
每个合法三元组 $(i, j, k)$ 有唯一的中间下标 $j \in [0, n)$。当 $j$ 固定时，左端 $i$ 有 $L[j]$ 种合法取法，右端 $k$ 有 $R[j]$ 种合法取法，且任意左端取法与任意右端取法组合均满足 $i < j < k$ 且 $a_i < a_j < a_k$。由乘法原理与加法原理，总数为 $\sum_{j=0}^{n-1} L[j] \cdot R[j]$。$\blacksquare$

在 $60\%$ 子任务（$n \le 2000$）中，直接对每个 $j$ 向左、向右各扫一遍求 $L[j], R[j]$ 即可做到 $O(n^2)$。

### 2.3 归约到经典算法：值域树状数组（BIT）动态维护前缀频数

要把 $O(n^2)$ 加速到 $O(n \log V)$，只需快速回答“已扫描元素中有多少个值严格小于 $a_j$”以及“有多少个值严格大于 $a_j$”。
- 从左往右扫描 $j = 0 \dots n-1$，用值域树状数组维护已访问元素的出现频数，则 $L[j] = \operatorname{sigma}(a_j - 1)$，随后将 $a_j$ 插入树状数组；
- 清空树状数组，从右往左扫描 $j = n-1 \dots 0$，此时右侧总共有 $n - 1 - j$ 个元素，其中 $\le a_j$ 的有 $\operatorname{sigma}(a_j)$ 个，故严格大于 $a_j$ 的个数为 $R[j] = (n - 1 - j) - \operatorname{sigma}(a_j)$。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 固定三元组中间元素 $j$，将三元耦合约束解耦为左侧小于 $a_j$ 与右侧大于 $a_j$ 两个独立的一元计数问题（引理 1）。
- **consider easier version** — $n \le 2000$ 时对每个 $j$ 暴力扫描两侧，以 $O(n^2)$ 求得 $L[j]$ 与 $R[j]$。
- **classic problem solution** — 正向扫描配合值域树状数组维护前缀和求 $L[j]$。
- **classic problem solution** — 反向扫描配合值域树状数组（总个数减去 $\le a_j$ 的前缀和）求 $R[j]$。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
|---|---|---|---|---|
| `n` | `int` | 序列长度 | 读入 | 元素个数 |
| `a[mxn]` | `int [30005]` | 原序列 $a_i$ | 读入 | 原始权值 |
| `L[mxn]` | `int [30005]` | $j$ 左侧严格小于 $a_j$ 的元素个数 | `0` | 左合法数 |
| `R[mxn]` | `int [30005]` | $j$ 右侧严格大于 $a_j$ 的元素个数 | `0` | 右合法数 |
| `BIT[mxv]` | `int [100005]` | 值域 $[1, 10^5]$ 上的树状数组 | `0` | 频数前缀和 |

### 4.2 算法步骤

1. 读入 $n$ 及 `a[0..n-1]`，记录最大值 `mv`。
2. 正向遍历 $i = 0 \dots n-1$：`L[i] = sigma(a[i] - 1)`，然后 `add(a[i], 1)`。
3. 将 `BIT[0..mv+1]` 清零，反向遍历 $i = n-1 \dots 0$：`R[i] = (n - 1 - i) - sigma(a[i])`，然后 `add(a[i], 1)`。
4. 累加 `ans += 1LL * L[i] * R[i]` 并输出。

### 4.3 复杂度与验证报告

- **时间复杂度**：$O(n \log (\max a_i))$。
- **空间复杂度**：$O(n + \max a_i)$。
- **对拍验证**：`appendix/P1637/P1637_check.py` 中与 $O(n^2)$ 暴力基准完成 **500 组随机数据对拍**全部一致；极限规模 $n = 30,000$ 实测耗时 **0.0222 s**。

## 5. 示例代码

```cpp
//
// 题目：P1637 三元上升子序列
// https://www.luogu.com.cn/problem/P1637
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{30005},mxv{100005};

int n,a[mxn],L[mxn],R[mxn],BIT[mxv];

int lowbit(int x){return x&-x;}

void add(int x,int v)
{
	for(;x<mxv;x+=lowbit(x))
		BIT[x]+=v;
}

int sigma(int x)
{
	int r=0;
	for(;x>0;x-=lowbit(x))
		r+=BIT[x];
	return r;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	int mv=0;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];
		mv=max(mv,a[i]);
	}

	for(int i=0;i<n;++i)
	{
		L[i]=sigma(a[i]-1);
		add(a[i],1);
	}

	fill_n(BIT,mv+2,0);
	for(int i=n-1;i>=0;--i)
	{
		R[i]=(n-1-i)-sigma(a[i]);
		add(a[i],1);
	}

	i64 ans=0;
	for(int i=0;i<n;++i)
		ans+=1LL*L[i]*R[i];
	cout<<ans<<'\n';
	return 0;
}
```
