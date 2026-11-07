# P1947 猜数

## 1. 重新读题

### 1.1 问题重述（人话版）

交互题。评测库心里想了一个 $k\in[1,n]$。你每次调用

```cpp
int Seniorious(int x);
```

会得到：

- $k<x$ 返回 $1$；
- $k=x$ 返回 $0$；
- $k>x$ 返回 $-1$。

你需要在**不超过 $c$ 次**调用内确定 $k$，并实现

```cpp
extern "C" int Chtholly(int n,int c);
```

返回猜到的 $k$。**不能向标准输出打印任何东西**（答案只能通过返回值给出）。

### 1.2 关键约束与规模暗示

- $2\le n\le 10^6$，$\min(20,n-1)\le c\le n$。
- 30% 数据 $c=n-1$：退化成线性扫描也能过。
- 100% 数据给出 $c\ge\min(20,n-1)$。而 $\lceil\log_2 10^6\rceil=20$，这个恰好就是二分所需的询问次数上界。
- 「$c$ 最小是 $20$，而 $n$ 最大 $10^6<2^{20}$」这句话本身就是在提示：**标准二分**即可，不需要任何优化。

---

## 2. 思维链（Chain-of-thought）

### 2.1 问题的本质：在一个有序集合里找目标

候选集合是 $\{1,2,\dots,n\}$，查询 `Seniorious(x)` 把候选集切成三块：

$$
\{1,\dots,x-1\},\quad\{x\},\quad\{x+1,\dots,n\},
$$

返回值直接告诉你 $k$ 落在哪一块。这正是「比较型查找」的模型。因为集合天然有序，任何比较查询最多只能把可能性分成三份、其中两份是非空区间，所以每问一次最优情况下能把区间缩小到约一半。

### 2.2 为什么二分是最优的

**引理 1（决策树下界）**：任何用这种三值比较确定 $k$ 的策略，最坏情况下至少需要 $\lceil\log_2 n\rceil$ 次询问。

**证明**：把策略画成一棵决策树：每个内部结点是一次询问（三个分支：小于 / 等于 / 大于），叶子是确定的答案。$n$ 个可能的 $k$ 必须落在**不同的叶子**上（否则两种取值会产生相同的回答序列，无法区分）。深度为 $d$ 的三叉树最多有 $3^d$ 个叶子，但由于「等于」分支一旦走到就立即结束，继续询问的只有小于 / 大于两个分支；沿着任何一条「不命中」的路径，每层最多把候选区间分成两个可能的非空子区间，故深度 $d$ 的树在「不命中」路径上最多区分 $2^d$ 个候选（命中分支只是一个单点）。要使 $n$ 个候选都可区分，必须 $2^d\ge n$，即 $d\ge\lceil\log_2 n\rceil$。$\square$

（严格地说，三值比较的决策树每个结点有三个孩子，但「等于」孩子是叶子。把等于孩子替换成一个占位叶子后，非叶子的有效分支数是 $2$，于是「内部结点数」至少是 $n-1$，深度至少 $\lceil\log_2 n\rceil$。这与比较排序的下界论证同型。）

### 2.3 二分达到下界

维护答案所在区间 $[l,r]$（闭区间，初始 $[1,n]$）。每轮取
$$
mid=\left\lfloor\frac{l+r}{2}\right\rfloor,
$$
询问 `Seniorious(mid)`：

- 返回 $0$：直接返回 $mid$；
- 返回 $1$（$k<mid$）：令 $r=mid-1$；
- 返回 $-1$（$k>mid$）：令 $l=mid+1$。

**引理 2（正确性）**：上述过程中始终有 $k\in[l,r]$；循环结束时 $l=r$，返回的正是 $k$。

**证明**：初始 $k\in[1,n]$。每轮依据返回值的三种情况，分别把区间限制到确实包含 $k$ 的那一侧（返回 $0$ 已直接命中）。归纳得不变式。当 $l=r$ 时区间只剩一个数，而不变式保证 $k$ 在其中，故 $k=l$。$\square$

**引理 3（询问次数上界）**：最多询问 $\lceil\log_2 n\rceil$ 次（命中那次也算在内）。

**证明**：设当前区间长度 $m=r-l+1$。若未命中，则 $mid$ 把区间分成左侧 $mid-l$ 个与右侧 $r-mid$ 个元素，两者都不超过 $\lceil m/2\rceil$：

- $m$ 为偶数时，两侧分别为 $m/2-1$ 与 $m/2$（或反之），最大值 $m/2=\lceil m/2\rceil$；
- $m$ 为奇数时，两侧都是 $(m-1)/2<\lceil m/2\rceil$。

于是每问一次（未命中），区间长度至少减半：$m_{t}\le\lceil m_{t-1}/2\rceil\le m_0/2^t+1$。严格地，$m_t\le\lceil n/2^t\rceil$。当 $2^t\ge n$ 时 $m_t=1$，循环结束。命中则更早结束。所以询问次数 $\le\lceil\log_2 n\rceil$。$\square$

### 2.4 与 $c$ 的关系

题目保证 $c\ge\min(20,n-1)$，而 $n\le10^6<2^{20}$，故
$$
\lceil\log_2 n\rceil\le20\le c
$$
（当 $n\le 10^6$ 时 $\lceil\log_2 n\rceil\le20$；当 $20>n-1$ 时，$\min(20,n-1)=n-1\ge\lceil\log_2 n\rceil$ 对 $n\ge2$ 成立，因为 $n-1\ge\log_2 n$）。因此二分一定在预算内完成，不需要用 $c$ 做任何调整。

### 2.5 交互题的实现纪律

- 只声明 `extern "C" int Seniorious(int);`，**不要**自己定义它；
- `Chtholly` 也必须用 `extern "C"` 导出，否则评测库链接不到；
- 绝对不能用 `cout` / `printf` 输出任何东西，包括调试信息——交互题的标准输出会污染评测；
- 不要在函数里 `return` 之外的方式「输出答案」。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 一次比较把有序候选集切成三块，其中「等于」块是单点。
- **classic problem solution** — 三值比较的决策树下界是 $\lceil\log_2 n\rceil$，二分恰好达到。
- **classic problem solution** — 维护闭区间 $[l,r]$，按返回值收缩，$l=r$ 时返回。
- **implementation discipline** — 只返回值、不打印；`extern "C"` 声明交互函数。

---

## 4. 真实解法（The Real Solution）

### 4.1 符号表

| 符号 | 类型 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `int` | 候选范围上界 | 参数 | 规模 |
| `c` | `int` | 询问次数预算（本题不需要使用） | 参数 | budget |
| `l,r` | `int` | 当前答案所在闭区间 | $1,n$ | left/right |
| `mid` | `int` | 本轮询问点 $\lfloor(l+r)/2\rfloor$ | 计算 | middle |
| `res` | `int` | `Seniorious(mid)` 的返回值 | — | result |

### 4.2 算法步骤

1. 令 $l=1$，$r=n$。
2. 当 $l<r$：
   1. $mid=l+\lfloor(r-l)/2\rfloor$；
   2. `res=Seniorious(mid)`；
   3. 若 `res==0` 返回 $mid$；
   4. 若 `res>0`（$k<mid$）令 $r=mid-1$；否则令 $l=mid+1$。
3. 返回 $l$。

### 4.3 复杂度分析

- **询问次数**：$\le\lceil\log_2 n\rceil\le20$，满足 $c$ 的限制。
- **时间 / 空间**：$O(\log n)$ / $O(1)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1947 猜数（交互题）
// https://www.luogu.com.cn/problem/P1947
#include<bits/stdc++.h>

extern "C" int Seniorious(int);

extern "C" int Chtholly(int n,int c)
{
	(void)c;
	int l{1},r{n};
	while(l<r)
	{
		int mid=l+(r-l)/2;
		int res=Seniorious(mid);
		if(res==0)
			return mid;
		if(res>0)
			r=mid-1;
		else
			l=mid+1;
	}
	return l;
}
```
