# P1088 [NOIP 2004 普及组] 火星人 — 题解

---

## 1. 重新阅读问题

火星人用 $1 \sim N$ 的一个排列表示一个整数。排列按 **字典序升序** 编号，字典序最小的排列 $123\cdots N$ 代表数字 $1$，次小排列代表 $2$，依此类推。给定当前火星人手指的排列（即一个 $1 \sim N$ 的排列）和一个很小的正整数 $M$，需要计算**当前排列在字典序顺序下向后数 $M$ 个**所得到的排列，并输出。

**数据范围**：$N \le 10000$，$M \le 100$。保证结果不超出 $N!$ 范围。

本质问题：**求一个排列在字典序全排列中的第 $M$ 个后继**。

---

## 2. 思维链

### 2.1 转化为求后继排列

问题等价于：

> 给定排列 $P$ 和正整数 $M$，求排列 $Q$，使得 $Q$ 在字典序全排列列表中恰好排在 $P$ 之后第 $M$ 个。

如果用 $\operatorname{rank}(P)$ 表示 $P$ 的字典序序号（从 $1$ 开始），则 $\operatorname{rank}(Q) = \operatorname{rank}(P) + M$。

### 2.2 关键观察：$M$ 很小

$M \le 100$ 是一个非常强的限制。这意味着我们不需要高效的 $O(N)$ 求第 $k$ 个排列算法，因为 **重复执行 $M$ 次 next_permutation 即可**。

每次 `next_permutation` 的时间复杂度为 $O(N)$，总复杂度 $O(N \cdot M) \le 10^6$，完全可以在 1 秒内运行完毕。

### 2.3 为什么选择多次 next_permutation

C++ 标准库 `std::next_permutation` 的实现原理是：

1. 从右向左找到第一个下降点 $i$：$P[i] < P[i+1]$
2. 从右向左找到第一个大于 $P[i]$ 的数 $P[j]$
3. 交换 $P[i]$ 与 $P[j]$
4. 翻转 $P[i+1 \dots N-1]$

单次时间复杂度 $O(N)$。$M$ 次共 $O(N \cdot M) = O(10^6)$，轻松通过 $N = 10000$ 的数据。

### 2.4 数学方法——阶乘数系

扩展思考：如果不限制 $M$ 的大小（例如 $M$ 可达 $10^9$），我们该怎么做？

此时需要使用 **阶乘数系（Factorial Number System, 也称 Lehmer 码）**。

**阶乘数系**：任何一个整数 $k$（$0 \le k < N!$）可以唯一表示为：

$$
k = a_{N-1} \cdot (N-1)! + a_{N-2} \cdot (N-2)! + \cdots + a_1 \cdot 1! + a_0 \cdot 0!
$$

其中 $0 \le a_i \le i$。

**Lehmer 码与排列的转换**：

- **排列 $\to$ Lehmer 码**：对每个位置 $i$，$a_i = $ 在 $P[i]$ 右侧比 $P[i]$ 小的元素个数。
- **Lehmer 码 $\to$ 排列**：维护一个有序集合 $\{1, 2, \dots, N\}$，对每个 $a_i$，取集合中第 $a_i$ 小的元素（0-indexed），然后移除该元素。

本题完整流程（数学方法）：

1. 将当前排列转化为 Lehmer 码 $L$
2. 将 Lehmer 码看作阶乘数系下的整数，计算 $k = \text{val}(L)$
3. 计算 $k' = k + M$
4. 将 $k'$ 转化为新的 Lehmer 码 $L'$
5. 将 $L'$ 转化为新排列

Lehmer 码的转化需要 $O(N^2)$ 的朴素实现，或 $O(N \log N)$ 的 Fenwick 树优化。

**但本题 $M$ 很小，没有必要使用数学方法。** 直接多次 `next_permutation` 是最简洁、最不易出错的解法。

### 2.5 正确性证明

**命题**：对排列 $P$ 连续执行 $M$ 次 `next_permutation` 得到的结果，等于字典序全排列中 $P$ 之后的第 $M$ 个排列。

**证明**：由 `next_permutation` 的定义可知，每次调用将当前排列变换为字典序意义下的**严格下一个排列**（即字典序中最小的大于当前排列的排列）。由于字典序全排列构成一个全序集，从 $P$ 出发，每执行一次 $next$ 操作，就沿着这个全序向后移动一步。执行 $M$ 次后，恰好到达第 $M$ 个后继位置。$\blacksquare$

---

## 3. 模式总结

| 要素 | 本题特征 |
|------|----------|
| **输入形式** | 一个排列 + 一个整数 $M$ |
| **目标** | 求排列在字典序下的第 $M$ 个后继 |
| **核心性质** | $M \le 100$，远小于 $N$ |
| **算法策略** | 直接重复调用 `next_permutation` $M$ 次 |
| **复杂度** | $O(N \cdot M)$，约 $10^6$ 次操作 |
| **替代策略** | 阶乘数系 + Lehmer 码，用于 $M$ 很大的场景 |

**模式识别信号**：凡是「排列 + 字典序 + 加一个数」或「求排列的第 $k$ 个后继」，都优先想到两种方案：
- 若 $k$ 很小 → 重复 next_permutation
- 若 $k$ 很大 → 阶乘数系

---

## 4. 正式解法

### 4.1 算法流程

```
输入 N, M 和排列 a[0..N-1]
for _ in 1..M:
    执行一次 next_permutation(a, a+N)
输出 a[0..N-1]
```

### 4.2 `next_permutation` 实现细节

伪代码：

```
function next_permutation(a[0..n-1]) -> bool:
    i = n - 2
    while i >= 0 and a[i] >= a[i+1]:
        i--
    if i < 0:           // 已是最后一个排列（递减序）
        return false
    j = n - 1
    while a[j] <= a[i]:
        j--
    swap(a[i], a[j])
    reverse(a[i+1..n-1])
    return true
```

### 4.3 复杂度分析

- **时间复杂度**：$O(N \cdot M)$。每次 `next_permutation` 在均摊意义下 $O(N)$，最坏也是 $O(N)$。$N \le 10000$，$M \le 100$，最多 $10^6$ 次比较/交换操作，完全可行。
- **空间复杂度**：$O(1)$ 额外空间（除输入数组外仅使用几个临时变量）。

### 4.4 符号表

| 符号 | 含义 |
|------|------|
| $N$ | 火星人手指数目（排列长度） |
| $M$ | 要加上的小整数 |
| $a$ | 当前排列（0-indexed 数组） |
| $i$ | 第一个下降点下标 |
| $j$ | 右侧第一个大于 $a[i]$ 的元素下标 |

---

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005};
int a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int N, M;
	cin >> N >> M;
	for (int i{0}; i < N; ++i)
	{
		cin >> a[i];
	}

	while (M--)
	{
		next_permutation(a, a + N);
	}

	for (int i{0}; i < N; ++i)
	{
		if (i) cout << ' ';
		cout << a[i];
	}
	cout << '\n';

	return 0;
}
```

### 代码说明

- 使用 `next_permutation` 标准库函数，直接解决核心问题
- 输入 $M \le 100$，外层循环最多 100 次，每次 $O(N)$
- 输出格式严格控制：相邻数字用单个空格分隔，行末无多余空格，末尾换行
- 严格遵守编码规范：仅 `bits/stdc++.h`、全局 `using namespace std`、`constexpr` 初始化数组大小带余量
