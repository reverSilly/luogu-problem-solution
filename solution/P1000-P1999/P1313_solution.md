# P1313 [NOIP 2011 提高组] 计算系数 — 完整题解

## Part 1. 重读题目（白话版）

给定一个二项式 $(by+ax)^k$，把它展开成多项式，要求其中 $x^n y^m$ 这一项的系数。

- 输入：5 个整数 $a, b, k, n, m$。
- 输出：所求系数对 $10007$ 取模后的结果。

**数据范围**：

- 30%：$0 \le k \le 10$（小数据，可以暴力枚举）。
- 50%：$a=1, b=1$（系数本身退化为二项式系数）。
- 100%：$0 \le k \le 1000$，$0 \le n,m \le k$，且 $n+m=k$，$0 \le a,b \le 10^6$。

注意 $n+m=k$ 这个约束非常关键：它说明 $x^n y^m$ 这一项是**确定存在**的（只要 $n+m=k$），不需要处理"不存在则系数为 0"的情况。

一句话：**算 $(by+ax)^k$ 展开后 $x^n y^m$ 的系数，模 $10007$。**

---

## Part 2. 思维链（发现过程，含完整证明）

### 第 1 步：探索它的性质 —— 这是一个标准的二项展开式

$(by+ax)^k$ 是一个**二项式的 $k$ 次幂**。这种形式在高中数学里就有名字：**二项式定理**直接给出展开式。

二项式定理的标准形式：

$$(A+B)^k = \sum_{i=0}^{k} \binom{k}{i} A^{k-i} B^{i}$$

这里 $A = by$，$B = ax$（或者反过来 $A=ax, B=by$，看怎么方便）。

我们要找 $x^n y^m$ 这一项的系数。观察 $x$ 和 $y$ 的来源：$x$ 只出现在 $ax$ 里，$y$ 只出现在 $by$ 里。所以**只有同时取到 $n$ 个 $ax$ 因子和 $m$ 个 $by$ 因子**的项才贡献 $x^n y^m$。

### 第 2 步：考虑更简单的版本 —— $a=b=1$ 时退化为二项式系数

先做 $a=b=1$ 的子任务：此时 $(y+x)^k$ 展开后 $x^n y^m$ 的系数就是 $\binom{k}{n}$（或 $\binom{k}{m}$，二者相等）。这是 50% 数据的解法，也是整道题的核心结构。

**核心结论**：无论 $a,b$ 是否为 1，系数都包含 $\binom{k}{n}$ 这个组合数因子，再乘以 $a$ 和 $b$ 的相应幂。

### 第 3 步：经典问题解法 —— 二项式定理

**二项式定理**（标准形式）：

$$(P+Q)^k = \sum_{i=0}^{k} \binom{k}{i} P^{k-i} Q^{i}$$

其中 $\binom{k}{i} = \dfrac{k!}{i!(k-i)!}$ 是二项式系数。

**应用**：令 $P = by$，$Q = ax$，代入：

$$(by+ax)^k = \sum_{i=0}^{k} \binom{k}{i} (by)^{k-i} (ax)^{i} = \sum_{i=0}^{k} \binom{k}{i} b^{k-i} a^{i} \, y^{k-i} x^{i}$$

我们要 $x^n y^m$，对照 $x^{i} y^{k-i}$，需要 $i=n$，$k-i=m$，即 $i=n$ 且 $k-n=m$。由题设 $n+m=k$，第二个条件自动满足。所以**唯一**贡献 $x^n y^m$ 的项是 $i=n$ 那一项：

$$\text{系数} = \binom{k}{n} \cdot b^{k-n} \cdot a^{n} = \binom{k}{n} \cdot b^{m} \cdot a^{n}$$

### 第 4 步：严格证明（每个引理都给完整证明）

**引理 1（唯一性）。** 在 $(by+ax)^k$ 的展开式中，$x^n y^m$ 这一项**恰好**由二项展开中 $i=n$ 的那一项贡献，不存在其他项也含 $x^n y^m$。

**证明。** 二项式定理把 $(by+ax)^k$ 写成 $k+1$ 个互不相同的单项式之和：

$$(by+ax)^k = \sum_{i=0}^{k} \binom{k}{i} b^{k-i} a^{i} \, y^{k-i} x^{i}$$

记第 $i$ 项为 $T_i = \binom{k}{i} b^{k-i} a^{i} \, y^{k-i} x^{i}$。

- 第 $i$ 项中 $x$ 的指数恰好是 $i$，$y$ 的指数恰好是 $k-i$。
- 我们要 $x$ 的指数为 $n$、$y$ 的指数为 $m$，即需要 $i=n$ 且 $k-i=m$。
- 由于 $i$ 取值唯一（$i$ 就是二项展开的求和下标），所以**只有 $i=n$ 这一项**满足条件。
- 又因 $n+m=k$（题设），$k-n=m$ 自动成立，所以 $i=n$ 这一项确实就是 $x^n y^m$。

而展开式里不同的 $i$ 给出不同的 $x^i y^{k-i}$ 单项式，这些单项式两两不可合并（指数对 $(i, k-i)$ 互不相同），所以最终多项式中 $x^n y^m$ 项的系数就是 $T_n$ 的系数，即 $\binom{k}{n} b^{m} a^{n}$。$\blacksquare$

**引理 2（二项式定理本身，供参考）。** $(P+Q)^k = \sum_{i=0}^{k} \binom{k}{i} P^{k-i} Q^{i}$。

**证明。** 用乘法原理。把 $(P+Q)^k$ 看成 $k$ 个括号 $(P+Q)$ 相乘：

$$\underbrace{(P+Q)(P+Q)\cdots(P+Q)}_{k \text{ 个}}$$

展开时，从每个括号里各选一个因子（$P$ 或 $Q$）相乘，$k$ 个括号就有 $2^k$ 种选法。

- 选到 $i$ 个 $Q$、$k-i$ 个 $P$ 的选法有 $\binom{k}{i}$ 种（从 $k$ 个位置中选 $i$ 个放 $Q$，其余放 $P$）。
- 每一种这样的选法得到的乘积都是 $P^{k-i} Q^{i}$。
- 所以 $P^{k-i} Q^{i}$ 这一项的总系数是 $\binom{k}{i}$。
- 对所有 $i=0,1,\dots,k$ 求和即得二项式定理。$\blacksquare$

**引理 3（取模运算与乘法的相容性）。** 对整数 $x, y$ 和正整数 $p$，有 $(xy \bmod p) \bmod p = ((x \bmod p) \cdot (y \bmod p)) \bmod p$；并且若 $x \equiv x' \pmod{p}$、$y \equiv y' \pmod{p}$，则 $xy \equiv x'y' \pmod{p}$。

**证明。** 设 $x = qp + r$（$r = x \bmod p$），$y = sp + s'$（$s' = y \bmod p$）。则

$$xy = (qp+r)(sp+s') = qp\cdot sp + qp\cdot s' + r\cdot sp + rs'$$

前三个项都含因子 $p$，模 $p$ 后为 $0$，故 $xy \equiv rs' \pmod{p}$，即 $xy \bmod p = (r \cdot s') \bmod p$。由 $r \equiv x \pmod{p}$、$s' \equiv y \pmod{p}$ 也立得相容性。$\blacksquare$

**引理 4（取模幂）。** 对整数 $x, k \ge 0$ 和正整数 $p$，$x^k \bmod p$ 等于把 $x$ 逐次相乘、每一步都对 $p$ 取模的结果（即 `for i in [0,k): r = r * (x mod p) % p`）。

**证明。** 用引理 3 归纳。设 $r_0 = x \bmod p$，$r_{i+1} = (r_i \cdot (x \bmod p)) \bmod p$。

- 基例 $i=0$：$r_0 = x \bmod p \equiv x^1 \pmod{p}$。
- 归纳步：设 $r_i \equiv x^{i+1} \pmod{p}$。则 $r_{i+1} = (r_i \cdot (x \bmod p)) \bmod p \equiv x^{i+1} \cdot x \pmod{p}$（引理 3），即 $r_{i+1} \equiv x^{i+2} \pmod{p}$。
- 归纳 $k$ 步得 $r_k \equiv x^{k+1} \pmod{p}$，即 $r_k = x^{k+1} \bmod p$。令 $k-1$ 次循环得 $x^k \bmod p$。$\blacksquare$

**总正确性定理。** 算法输出的 $\left[\binom{k}{n} \cdot a^{n} \cdot b^{m}\right] \bmod 10007$ 恰好是 $(by+ax)^k$ 展开后 $x^n y^m$ 项的系数对 $10007$ 取模。

**证明。** 由引理 1，真实系数（未取模）为 $C = \binom{k}{n} a^{n} b^{m}$。算法用模意义下的乘法（引理 3）和幂（引理 4）分别算出 $\binom{k}{n} \bmod p$、$a^{n} \bmod p$、$b^{m} \bmod p$，再把三者相乘并取模。由引理 3，每一步的取模都不改变最终模 $p$ 的值，所以最终结果 $= C \bmod p$。$\blacksquare$

### 第 5 步：定下算法

$C = \binom{k}{n} a^{n} b^{m} \bmod 10007$，其中 $k \le 1000$，$p = 10007$。

**计算 $\binom{k}{n}$**：$k$ 只有 1000，直接递推：

$$\binom{k}{i} = \binom{k}{i-1} \cdot \frac{k-i+1}{i}$$

但这里有除法，模意义下要处理逆元。**更简单的方法**：直接算阶乘，$k \le 1000$，阶乘数组很小：

$$\binom{k}{n} = \frac{k!}{n!\,(k-n)!} \bmod p$$

用阶乘数组 `fac[i] = i! mod p`，再求 $(n! \cdot (k-n)!)^{-1} \bmod p$ 即可。

**等等**——这里有个坑：$p=10007$ 是质数吗？是的，10007 是质数。但 $k \le 1000 < 10007 = p$，所以 $k!$ 不含因子 $p$，阶乘在模 $p$ 下**可逆**，可以用费马小定理求逆元 $x^{-1} \equiv x^{p-2} \pmod{p}$。

**更简单的做法**（完全避免逆元）：用杨辉三角（Pascal 三角）递推 $\binom{k}{n}$。$\binom{k}{i} = \binom{k-1}{i-1} + \binom{k-1}{i}$，$k \le 1000$ 的二维数组 $O(k^2)$ 完全没问题（1000×1000 = 10⁶）。

**最终方案**：

1. 用杨辉三角递推 `C[k][i]`，得到 $\binom{k}{n}$。
2. 用快速幂或逐次乘法算 $a^n \bmod p$ 和 $b^m \bmod p$。
3. 三者相乘取模。

时间 $O(k^2 + k \log p)$，空间 $O(k^2)$，$k \le 1000$ 完全够用。

### 第 6 步：复杂度与手推验证

- **时间**：杨辉三角 $O(k^2) = O(10^6)$；两个幂 $O(k) = O(1000)$（逐次乘法即可，$k$ 很小）。总 $O(k^2)$。
- **空间**：二维数组 $O(k^2) = O(10^6)$。

**手推验证**（$a=1, b=1, k=3, n=1, m=2$）：

$(y+x)^3 = y^3 + 3y^2x + 3yx^2 + x^3$

$x^1 y^2$ 的系数 = $3 = \binom{3}{1}$。公式：$\binom{3}{1} \cdot 1^1 \cdot 1^2 = 3$。✔

**手推验证**（$a=2, b=3, k=2, n=1, m=1$）：

$(3y+2x)^2 = 9y^2 + 12xy + 4x^2$

$x^1 y^1$ 的系数 = $12$。公式：$\binom{2}{1} \cdot 2^1 \cdot 3^1 = 2 \cdot 2 \cdot 3 = 12$。✔

---

## Part 3. 模式总结（按实际思考顺序）

- **explore its property** —— $(by+ax)^k$ 是标准的二项式 $k$ 次幂，$x$ 只来自 $ax$、$y$ 只来自 $by$，所以 $x^n y^m$ 只能由"选 $n$ 个 $ax$ + 选 $m$ 个 $by$"贡献。
- **consider easier version** —— $a=b=1$ 时系数退化为 $\binom{k}{n}$，50% 子任务的解法，也是核心结构。
- **classic problem solution** —— 二项式定理：$(P+Q)^k = \sum \binom{k}{i} P^{k-i} Q^i$，令 $P=by, Q=ax$ 得系数 $= \binom{k}{n} a^n b^m$。
- **reduce to easier problem** —— 把"求多项式系数"归约为"算一个二项式系数 + 两个幂"三个小问题，都在 $O(k^2)$ 内可解。
- **classic problem solution** —— 二项式系数用杨辉三角（Pascal 三角）$O(k^2)$ 递推，避免模意义下求逆元的麻烦。

---

## Part 4. 真正的解法（含记号表）

### 记号表

| 符号 | 含义 | 初始化 | 记忆法 |
|---|---|---|---|
| `a, b, k, n, m` | 输入：系数 $a, b$，幂次 $k$，$x$ 的指数 $n$，$y$ 的指数 $m$ | 读入 | 基本量 |
| `p` | 模数 $= 10007$ | 常量 | mod |
| `mxk` | 杨辉三角数组尺寸 $= k+5$（$k \le 1000$，取 $1005$） | 常量 | 数组松弛 |
| `C[mxk][mxk]` | `C[i][j]` = $\binom{i}{j} \bmod p$ | `C[0][0]=1`，其余按 Pascal 递推 | Pascal 三角 |
| `ans` | 最终答案 $\binom{k}{n} a^n b^m \bmod p$ | 算出 | answer |

> 记忆口诀：**`C[i][j]` = $\binom{i}{j} \bmod p$，杨辉三角一层层填下来；最终答案 = `C[k][n] * (a^n mod p) * (b^m mod p) mod p`**。

### 算法步骤

1. 读 $a, b, k, n, m$。
2. 杨辉三角递推 `C`：`C[0][0]=1`；`for i in [1, k]: C[i][0]=1; for j in [1, i]: C[i][j]=(C[i-1][j-1]+C[i-1][j])%p;`
3. 算 $a^n \bmod p$：`i64 pa=1; for i in [0, n): pa=pa*(a%p)%p;`
4. 算 $b^m \bmod p$：`i64 pb=1; for i in [0, m): pb=pb*(b%p)%p;`
5. `ans = (C[k][n] * pa % p) * pb % p;`
6. 输出 `ans`。

### 复杂度与验证

- **时间** $O(k^2)$（杨辉三角 $k^2$ 次加法取模，$k \le 1000$ 约 $10^6$ 次操作）；**空间** $O(k^2)$。
- 小样例（$a=1, b=1, k=3, n=1, m=2$）手推输出 $3$，与展开式系数一致 ✔。
- 小样例（$a=2, b=3, k=2, n=1, m=1$）手推输出 $12$，与展开式系数一致 ✔。

---

## Part 5. 示例代码（C++，遵循风格规范）

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int p{10007};
constexpr int mxk{1005};

int k,n,m;
int C[mxk][mxk];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int a,b;
	cin>>a>>b>>k>>n>>m;

	C[0][0]=1;
	for(int i=1;i<=k;++i)
	{
		C[i][0]=1;
		for(int j=1;j<=i;++j)
			C[i][j]=(C[i-1][j-1]+C[i-1][j])%p;
	}
	i64 pa=1,pb=1;
	for(int i=0;i<n;++i)
		pa=pa*(a%p)%p;
	for(int i=0;i<m;++i)
		pb=pb*(b%p)%p;
	int ans=(int)((i64)C[k][n]*pa%p*pb%p);
	cout<<ans<<'\n';
	return 0;
}
```

**实现要点 / 坑：**

- **$a, b$ 先取模**：$a, b \le 10^6 > p = 10007$，直接相乘会超过 `int`，所以每个因子先 `%p` 再乘，用 `i64`（`long long`）存中间量防溢出。
- **杨辉三角避免逆元**：$k \le 1000$，二维数组 $10^6$ 个 `int` 约 4 MB，完全放得下。直接用 Pascal 递推，不需要在模意义下求阶乘逆元（虽然 $p=10007$ 是质数、$k<p$ 时逆元存在，但杨辉三角更直观、更不容易出错）。
- **$n+m=k$ 的约束**：题设保证 $x^n y^m$ 项存在，不需要判边界。若 $n$ 或 $m$ 越界（不在 $[0,k]$），则系数为 0，但本题不需要处理。
- **`C[0][0]=1` 初始化**：杨辉三角的起点，其余元素按递推公式填。全局数组默认 0，所以 `C[0][0]` 必须显式赋 1。