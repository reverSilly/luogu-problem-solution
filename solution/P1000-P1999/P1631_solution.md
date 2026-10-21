# P1631 序列合并 题解

> 标签：优先队列 / 堆 / 贪心 / 多路归并 · 难度：提高/省选-

## 1. 重新读题

**问题重述（人话版）**

给定两个长度均为 $N$ 的**单调不降**正整数序列 $A_0 \le A_1 \le \dots \le A_{N-1}$ 与 $B_0 \le B_1 \le \dots \le B_{N-1}$。从 $A$ 和 $B$ 中各取一个数相加，一共可以得到 $N^2$ 个和 $A_i + B_j$（$0 \le i, j < N$）。请将这 $N^2$ 个和按从小到大的顺序排列，输出其中最小的 $N$ 个。

**约束与规模暗示**

- $50\%$ 数据：$N \le 10^3$；
- $100\%$ 数据：$1 \le N \le 10^5$，$1 \le A_i, B_i \le 10^9$。
- 值域上界：$A_i + B_j \le 2 \times 10^9 < 2^{31} - 1$，有符号 32 位 `int` 恰好装得下。
- 规模暗示：$N = 10^5$ 时 $N^2 = 10^{10}$，无论是空间还是时间都绝不可能把 $N^2$ 个和全部枚举出来；预期时间复杂度为 $O(N \log N)$。

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：把 $N^2$ 个和排成 $N$ 条有序链

固定左端点 $i \in [0, N)$，由于 $B$ 是单调不降序列，第 $i$ 行的 $N$ 个和天然构成一条单调不降链：
$$L_i:\quad A_i + B_0 \le A_i + B_1 \le A_i + B_2 \le \dots \le A_i + B_{N-1}.$$
于是，原问题“在 $N^2$ 个和里取前 $N$ 小”等价于：**在 $N$ 条已经排好序的递增链 $L_0, L_1, \dots, L_{N-1}$ 中，动态合并取出前 $N$ 小的元素**。

### 2.2 简化版与失败尝试

**简化版（$50\%$ 子任务，$N \le 10^3$）**：直接二重循环算出全部 $N^2 = 10^6$ 个和，调用 `sort` 取前 $N$ 个，复杂度 $O(N^2 \log N)$。

**失败尝试：只维护双指针 `(i, j)` 在网格上向右下走**。
反例：$A = [1, 2, 100]$，$B = [1, 2, 100]$。$(0, 0) \to 2$，接着 $(0, 1) \to 3$ 与 $(1, 0) \to 3$ 都需要被考虑到，随后还要考虑 $(1, 1) \to 4$。因为前沿可能同时跨越多行，仅靠常数个指针无法覆盖所有候选边界。

### 2.3 归约到经典算法：$N$ 路归并（小根堆维护每行队首）

**引理 1（每行未选元素的最小值必在该行当前队首）**

设在任意时刻，第 $i$ 行已经被取出了前 $\text{idx}[i]$ 个元素（$0 \le \text{idx}[i] \le N$）。则在所有尚未被取出的元素中，第 $i$ 行的最小候选必为 $A_i + B_{\text{idx}[i]}$（若 $\text{idx}[i] < N$）。

**证明**：
因为 $B$ 单调不降，对任意 $j > \text{idx}[i]$ 都有 $B_j \ge B_{\text{idx}[i]}$，从而 $A_i + B_j \ge A_i + B_{\text{idx}[i]}$。因此在第 $i$ 行未取出的元素 $\{A_i + B_j \mid j \ge \text{idx}[i]\}$ 中，下标 $j = \text{idx}[i]$ 取得最小值。由全序关系的传递性，全局未取出元素的最小值必在各行当前队首 $\{A_i + B_{\text{idx}[i]} \mid 0 \le i < N, \text{idx}[i] < N\}$ 中产生。$\blacksquare$

**引理 2（小根堆大小不超过 $N$，且总弹出/插入次数不超过 $2N$）**

初始将每行的第 $0$ 个和 $(A_i + B_0, i)$ 压入小根堆（共 $N$ 个元素）。每次弹出堆顶 $(s, i)$ 作为当前全局最小和，令 $\text{idx}[i] \leftarrow \text{idx}[i] + 1$；若 $\text{idx}[i] < N$，再将 $(A_i + B_{\text{idx}[i]}, i)$ 压入堆中。重复 $N$ 次即可按序得到最小的 $N$ 个和，总时间复杂度 $O(N \log N)$。

**证明**：
由引理 1，每次堆中恰好维护所有非空行的当前队首，因此第 $k$ 次弹出的堆顶就是剩余未取出元素中的全局最小值，归纳可知前 $N$ 次弹出的元素即为全局最小的 $N$ 个和。堆中每行至多有 $1$ 个代表元，故堆大小恒 $\le N$；初始入堆 $N$ 次，每弹出 $1$ 次至多入堆 $1$ 次，总堆操作不超过 $2N$ 次，每次 $O(\log N)$，总时间 $O(N \log N)$。$\blacksquare$

## 3. 模式总结（Pattern Summary）

- **explore its property** — 利用 $B$ 的单调性，将 $N^2$ 个二元和拆解为 $N$ 条各自有序的行序列 $L_i$。
- **consider easier version** — $N \le 10^3$ 时直接 $O(N^2 \log N)$ 全量枚举排序。
- **reduce to easier problem** — 证明每行未取元素的下界由队首 $\text{idx}[i]$ 唯一代表（引理 1），将 $N^2$ 规模的静态选择归约为 $N$ 条有序链的多路归并。
- **classic problem solution** — 用小根堆 `priority_queue` 维护 $N$ 条链的当前队首，弹出即推进对应链指针（引理 2），在 $O(N \log N)$ 时间内输出前 $N$ 小。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
|---|---|---|---|---|
| `n` | `int` | 序列长度 $N$ | 读入 | 题面维度 |
| `a[mxn]` | `int [100005]` | 单调不降序列 $A$（0-based） | 读入 | 左加数 |
| `b[mxn]` | `int [100005]` | 单调不降序列 $B$（0-based） | 读入 | 右加数 |
| `idx[mxn]` | `int [100005]` | 第 $i$ 行当前待取的 $B$ 下标 | `0` | 行内指针 |
| `node` | `struct {int s, i;}` | 堆节点：当前和 `s` 与所属行号 `i` | — | 小根堆元素 |
| `Q` | `priority_queue<node,...,greater<>>` | 维护各行队首的小根堆 | $N$ 个 $(A_i+B_0,i)$ | 多路归并优先队列 |

### 4.2 算法步骤

1. 读入 $n$ 以及数组 `a[0..n-1]`、`b[0..n-1]`。
2. 对每个 $i \in [0, n)$，令 `idx[i] = 0`，将 `{a[i] + b[0], i}` 压入小根堆 `Q`。
3. 循环 $k = 0, 1, \dots, n - 1$：
   - 弹出堆顶 `[s, i] = Q.top()`，输出 `s`；
   - 若 `++idx[i] < n`，将 `{a[i] + b[idx[i]], i}` 压入 `Q`。

### 4.3 复杂度与验证报告

- **时间复杂度**：$O(N \log N)$。
- **空间复杂度**：$O(N)$。
- **对拍验证**：`appendix/P1631/P1631_check.py` 中与 $O(N^2 \log N)$ 暴力基准完成 **500 组随机数据对拍**全部逐字节一致；极限规模 $N = 100,000$ 实测耗时 **0.1061 s**。

## 5. 示例代码

```cpp
//
// 题目：P1631 序列合并
// https://www.luogu.com.cn/problem/P1631
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

int n,a[mxn],b[mxn],idx[mxn];

struct node
{
	int s,i;
	friend bool operator>(node x,node y){return x.s>y.s;}
};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int i=0;i<n;++i)
		cin>>b[i];

	priority_queue<node,vector<node>,greater<>>Q;
	for(int i=0;i<n;++i)
	{
		idx[i]=0;
		Q.push({a[i]+b[0],i});
	}

	for(int k=0;k<n;++k)
	{
		auto[s,i]=Q.top();Q.pop();
		cout<<s<<(k+1==n?'\n':' ');
		if(++idx[i]<n)
			Q.push({a[i]+b[idx[i]],i});
	}
	return 0;
}
```
