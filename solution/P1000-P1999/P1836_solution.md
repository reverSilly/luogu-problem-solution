# P1836 数页码

> 难度：提高/省选- · 标签：数位 DP、按位统计

## 1. 重新读题

### 1.1 问题重述（人话版）

把 $1,2,3,\dots,n$ 的十进制写法首尾相接，求所有出现过的数字字符之和。等价地：对每个 $k\ge 1$，统计 $1\sim n$ 中所有数的第 $k$ 个十进制位（个位是第 $1$ 位）上数字的总和，再全部相加。例如 $123$ 贡献 $1+2+3=6$。

### 1.2 关键约束与规模暗示

- $1\le n\le 10^9$。逐个数位求和是 $O(n\log n)$，$n=10^9$ 直接超时，必须 $O(\log n)$ 或 $O(\log^2 n)$。
- 答案量级：$1\sim 10^9$ 的数字和约为 $4.05\times 10^{10}$，超过 $2^{31}-1$，必须用 64 位整数（`long long`）。
- $n$ 最多 10 位，位数很少，这暗示「按十进制位分别处理」：每一位的贡献只与该位左边、右边的数字有关，而这两部分可以 $O(1)$ 算出。

## 2. 思维链（Chain-of-thought）

### 2.1 先试最直白的做法，确认它死在哪里

枚举 $x=1\dots n$，把 $x$ 反复 `%10` 累加。复杂度 $O(n\log n)$，$n=10^9$ 时约 $9\times 10^9$ 次取模，必然超时。所以只能「不枚举数，而是枚举位置」。

### 2.2 把问题拆成「每个位置的数字和」

数字和是可加的：总和 $=\sum_{k\ge 1}\bigl(\text{第 }k\text{ 位上所有数字之和}\bigr)$。于是只需解决一个子问题：

> **子问题**：固定位置 $p=10^{k-1}$（个位 $p=1$，十位 $p=10$，……），求 $1\sim n$ 的所有数在第 $p$ 位上的数字之和。

### 2.3 补前导零不改变答案

把 $1\sim n$ 的每个数都在左边补零，补到与 $n$ 相同的位数。补上的零对该位数字和的贡献是 $0$，所以答案不变。这样第 $p$ 位就变成了一个纯粹按周期循环的序列：从 $0$ 开始，第 $p$ 位以 $10p$ 为周期，依次取 $00\cdots0$（$p$ 次）、$11\cdots1$（$p$ 次）、……、$99\cdots9$（$p$ 次）。

### 2.4 引理：固定位置的单周期贡献

**引理 1（一个完整周期的数字和是 $45p$）**

在长度为 $10p$ 的任意一个完整周期内（即第 $p$ 位的数字从 $0$ 循环到 $9$ 各 $p$ 次），第 $p$ 位上的数字之和恰为 $45p$。

**证明**：一个完整周期里，第 $p$ 位取每个数字 $d\in\{0,\dots,9\}$ 恰好 $p$ 次（因为低位共有 $p$ 种取值，每种取值配合一个固定的 $d$ 出现一次）。因此该位数字和 $=\sum_{d=0}^{9}d\cdot p=45p$。$\blacksquare$

### 2.5 引理：把 $0\sim n$ 分解成整周期 + 整块 + 残块

设 $p=10^{k-1}$，令

$$
\text{higher}=\left\lfloor\frac{n}{10p}\right\rfloor,\qquad
\text{cur}=\left\lfloor\frac{n}{p}\right\rfloor\bmod 10,\qquad
\text{lower}=n\bmod p .
$$

**引理 2（第 $p$ 位的贡献公式）**

$$
S_p=\text{higher}\cdot 45p+\frac{\text{cur}(\text{cur}-1)}{2}\,p+\text{cur}\cdot(\text{lower}+1).
$$

**证明**：把 $0,1,\dots,n$ 这 $n+1$ 个数按第 $p$ 位分成三段。

1. **完整大周期**：前 $\text{higher}$ 个长度为 $10p$ 的完整周期，即 $0\sim \text{higher}\cdot 10p-1$。由引理 1，每段贡献 $45p$，共 $\text{higher}\cdot 45p$。
2. **当前大周期内的完整小块**：剩下从 $\text{higher}\cdot 10p$ 到 $\text{higher}\cdot 10p+\text{cur}\cdot p-1$ 的 $\text{cur}$ 个长度为 $p$ 的小块，第 $p$ 位依次固定为 $0,1,\dots,\text{cur}-1$，每块贡献 $d\cdot p$。合计 $\bigl(0+1+\cdots+(\text{cur}-1)\bigr)p=\frac{\text{cur}(\text{cur}-1)}{2}p$。
3. **残块**：最后 $\text{lower}+1$ 个数（从 $\text{higher}\cdot 10p+\text{cur}\cdot p$ 到 $n$）第 $p$ 位都等于 $\text{cur}$，贡献 $\text{cur}\cdot(\text{lower}+1)$。

三段互不重叠且覆盖了 $0\sim n$，相加即得引理。$\blacksquare$

**为什么 $0$ 可以算进来**：$0$ 的每一位都是 $0$，对任何 $S_p$ 的贡献都是 $0$，所以统计 $0\sim n$ 与统计 $1\sim n$ 的结果相同。这一点让上面的「从 $0$ 开始的整周期分解」成立，而不必单独处理 $1$ 的边界。

### 2.6 复杂度

$p$ 依次取 $1,10,10^2,\dots\le n$，共 $O(\log_{10} n)\le 10$ 次，每次 $O(1)$，总时间 $O(\log n)$，空间 $O(1)$。

### 2.7 一次容易犯的错误

- 用 `int` 存答案：$n=10^9$ 时答案是 $40500000001>2^{31}-1$，会溢出成负数。必须 `long long`。
- 循环条件写成 `p<n`：会漏掉最高位所在的那一个 $p$；应为 `p<=n`，且 $p$ 每次乘 $10$ 不会溢出（$p\le 10^9$，再乘一次循环即退出，仍在 `long long` 范围内）。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 数字和按「十进制位」可加，转化为每个位置上的数字和。
- **reduce to easier problem** — 补前导零后，固定位置的数字序列变成严格的 $10p$ 周期序列。
- **classic problem solution** — 完整周期贡献 $45p$，把 $0\sim n$ 拆成「完整大周期 + 当前周期的完整小块 + 残块」三部分 $O(1)$ 计算。
- **explore its property** — 位数只有 $O(\log n)$，逐位累加即得答案。

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `i64` | 页码上界 | 读入 | $\le 10^9$ |
| `p` | `i64` | 当前处理的位权 $10^{k-1}$ | $1$，每轮乘 $10$ | position |
| `higher` | `i64` | $n$ 在当前位左边的高位部分 $\lfloor n/(10p)\rfloor$ | 每轮计算 | 高位 |
| `cur` | `i64` | $n$ 在当前位上的数字 | 每轮计算 | current digit |
| `lower` | `i64` | $n$ 在当前位右边的低位部分 $n\bmod p$ | 每轮计算 | 低位 |
| `ans` | `i64` | 所有位的数字和 | $0$ | 输出它 |

### 4.2 算法步骤

1. 读入 `n`。
2. 令 `p=1`；当 `p<=n` 时循环：
   1. `higher=n/(p*10)`，`cur=n/p%10`，`lower=n%p`；
   2. `ans += higher*45*p + cur*(cur-1)/2*p + cur*(lower+1)`；
   3. `p*=10`。
3. 输出 `ans`。

### 4.3 复杂度

- 时间 $O(\log_{10} n)$，至多 10 轮。
- 空间 $O(1)$。

## 5. 示例代码

```cpp
//
// 题目：P1836 数页码
// https://www.luogu.com.cn/problem/P1836
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	i64 n;
	cin>>n;
	i64 ans{0};
	for(i64 p=1;p<=n;p*=10)
	{
		i64 higher=n/(p*10);
		i64 cur=n/p%10;
		i64 lower=n%p;
		ans+=higher*45*p+cur*(cur-1)/2*p+cur*(lower+1);
	}
	cout<<ans<<'\n';
	return 0;
}
```
