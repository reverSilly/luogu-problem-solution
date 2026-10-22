# P1762 偶数

## 1. 重新读题

给定一个正整数 $n$（$1 \le n \le 10^{15}$），求杨辉三角形前 $n$ 行（即第 $0$ 行到第 $n-1$ 行，共 $\frac{n(n+1)}{2}$ 个数）中**偶数**的个数对 $1000003$ 取模后的结果。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：正难则反，用卢卡斯定理（Lucas' Theorem）数奇数！

杨辉三角前 $n$ 行的总元素个数是现成的：
$$\text{Total}(n) = \sum_{r=0}^{n-1} (r + 1) = \frac{n(n + 1)}{2}.$$
直接数偶数没有乘法分解性质，但**数奇数**极其简单！因为 $\binom{r}{c} \equiv 1 \pmod 2$ 可由模 $2$ 的 Lucas 定理直接刻画！

**引理 1（单行奇数个数的二进制 1 个数定理）**：在杨辉三角的第 $r$ 行（0-based，含 $\binom{r}{0}, \binom{r}{1}, \dots, \binom{r}{r}$）中，奇数的个数恰好等于
$$2^{\operatorname{popcount}(r)},$$
其中 $\operatorname{popcount}(r)$ 表示 $r$ 的二进制表示中 $1$ 的个数。

**证明**：
由素数 $p = 2$ 的 Lucas 定理，设 $r = \sum_{k \ge 0} r_k 2^k$，$c = \sum_{k \ge 0} c_k 2^k$（$r_k, c_k \in \{0, 1\}$），则：
$$\binom{r}{c} \equiv \prod_{k \ge 0} \binom{r_k}{c_k} \pmod 2.$$
由于 $\binom{0}{1} = 0$，乘积模 $2$ 为 $1$ 当且仅当对每一个二进制位 $k$ 都有 $c_k \le r_k$（即 $c$ 是 $r$ 的二进制子集）：
- 若 $r_k = 0$，则 $c_k$ 只能取 $0$（$1$ 种选择）；
- 若 $r_k = 1$，则 $c_k$ 可取 $0$ 或 $1$（$2$ 种选择）。
各二进制位独立，故满足条件的 $c \in [0, r]$ 共有 $2^{\operatorname{popcount}(r)}$ 个。$\blacksquare$

### 2.2 前缀奇数和的数位倍增递推（附完整证明）

**引理 2（前缀奇数和 $F(n)$ 的 $O(\log n)$ 计算公式）**：定义 $F(n) = \sum_{r=0}^{n-1} 2^{\operatorname{popcount}(r)}$。若 $n$ 的二进制表示中为 $1$ 的各位从高到低依次为 $2^{b_1} > 2^{b_2} > \dots > 2^{b_t}$（即 $n = \sum_{j=1}^t 2^{b_j}$），则：
$$F(n) = \sum_{j=1}^{t} 2^{j-1} \cdot 3^{b_j}.$$

**证明**：
对最高位归纳：当 $n = 2^b$ 时，$[0, 2^b - 1]$ 恰好遍历所有 $b$ 位二进制数，由二项式定理有 $\sum_{r=0}^{2^b - 1} 2^{\operatorname{popcount}(r)} = \sum_{k=0}^b \binom{b}{k} 2^k = (1 + 2)^b = 3^b$。
当 $n = 2^{b_1} + m$（$0 \le m < 2^{b_1}$）时，将区间 $[0, n)$ 拆分为 $[0, 2^{b_1})$ 与 $[2^{b_1}, 2^{b_1} + m)$：
- 前半区间之和为 $3^{b_1}$；
- 后半区间中每个数 $r = 2^{b_1} + x$（$0 \le x < m$）满足 $\operatorname{popcount}(r) = 1 + \operatorname{popcount}(x)$，故 $2^{\operatorname{popcount}(r)} = 2 \cdot 2^{\operatorname{popcount}(x)}$，求和得 $2 F(m)$。
递推展开 $F(2^{b_1} + m) = 3^{b_1} + 2 F(m)$ 即得 $F(n) = \sum_{j=1}^t 2^{j-1} \cdot 3^{b_j}$。用总数 $\frac{n(n+1)}{2}$ 减去 $F(n)$ 并对 $1000003$ 取模即为所求！$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 正难则反，用前 $n$ 行总数 $\frac{n(n+1)}{2}$ 减去奇数个数。
- **apply Lucas' theorem** — 证明第 $r$ 行奇数个数为 $2^{\operatorname{popcount}(r)}$（引理 1）。
- **classic problem solution** — 按 $n$ 的二进制位拆分前缀区间，用 $F(n) = \sum_{j=1}^t 2^{j-1} 3^{b_j}$ 在 $O(\log n)$ 时间内算出奇数总数（引理 2）。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `i64` | 行数 $n$ | 标准输入 | rows |
| `p3[k]` | `i64 [60]` | $3^k \bmod 1000003$ | 预处理 | powers of 3 |
| `tot` | `i64` | 总元素个数 $\frac{n(n+1)}{2} \bmod 1000003$ | `__int128` 计算 | total entries |
| `odd` | `i64` | 奇数总个数 $F(n) \bmod 1000003$ | 按位累加 | odd entries |

### 4.2 算法步骤

1. 读入 $n$，预处理 $3^0, 3^1, \dots, 3^{55} \pmod{1000003}$。
2. 计算总数 `tot = ((__int128)n * (n + 1) / 2) % mod`。
3. 令系数 `coef = 1`，从高位 $k = 55$ 递减至 $0$：若 `(n >> k) & 1` 为真，令 `odd = (odd + coef * p3[k]) % mod`，并令 `coef = coef * 2 % mod`。
4. 输出 `(tot - odd + mod) % mod`。

### 4.3 复杂度分析

- **时间复杂度**：仅扫描 $n$ 的 $\le 55$ 个二进制位，时间复杂度为 $\Theta(\log n)$。
- **空间复杂度**：常数级数组，空间复杂度为 $\Theta(1)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1762 偶数
// https://www.luogu.com.cn/problem/P1762
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr i64 mod{1000003};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	i64 n;
	cin>>n;

	i64 p3[60];
	p3[0]=1;
	for(int i=1;i<60;++i)
		p3[i]=p3[i-1]*3%mod;

	i64 tot=((__int128)n*(n+1)/2)%mod;
	i64 odd=0,coef=1;
	for(int k=55;k>=0;--k)
		if((n>>k)&1)
		{
			odd=(odd+coef*p3[k])%mod;
			coef=coef*2%mod;
		}

	cout<<(tot-odd+mod)%mod<<'\n';
	return 0;
}
```
