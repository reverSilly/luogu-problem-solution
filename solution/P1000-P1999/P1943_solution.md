# P1943 Local Maxima

## 1. 重新读题

### 1.1 问题重述（人话版）

在 $1\sim n$ 的所有 $n!$ 个排列中随机等概率取一个。称位置 $i$ 上的数是**局部最大值**，当且仅当它比它**前面所有数**都大（注意：不是只比前一个数大）。注意第一个数前面没有数，按定义它总是局部最大值。

求局部最大值个数的期望，输出实数，保留八位小数。

### 1.2 关键约束与规模暗示

- $30\%$：$n\le 10$，可以枚举全排列。
- $80\%$：$n\le 10^6$，可以直接 $O(n)$ 累加。
- $100\%$：$1\le n<2^{31}$，$O(n)$ 都过不了（$2\times10^9$ 次循环）。
- 输出只要 $8$ 位小数，而 $n$ 只以 $\ln n$ 的形式进入答案，说明满分做法必然是**调和级数的渐近展开**，用小 $n$ 直接求和、大 $n$ 用公式。

---

## 2. 思维链（Chain-of-thought）

### 2.1 从「期望个数」想到「指示变量」

直接求「局部最大值个数的分布」显然不现实：$n!$ 个排列。但题目只问**期望**，而期望对「个数」天然可拆。

令
$$
X_i=\begin{cases}1,&\text{位置 }i\text{ 是局部最大值},\\0,&\text{否则,}\end{cases}
$$
则局部最大值个数 $X=\sum_{i=1}^n X_i$。

**引理 1（期望的线性性）**：
$$
\mathbb E[X]=\sum_{i=1}^n \mathbb E[X_i]=\sum_{i=1}^n \Pr[X_i=1].
$$

**证明**：期望线性性对任意随机变量（不要求独立）都成立：
$$
\mathbb E\Bigl[\sum_{i=1}^n X_i\Bigr]=\sum_{i=1}^n\mathbb E[X_i].
$$
这是有限求和的线性性，逐项展开定义即可。又因为 $X_i$ 只取 $0/1$，$\mathbb E[X_i]=\Pr[X_i=1]$。$\square$

这一步的意义：**不用管各位置是否独立**，只要求出每个位置单独成为局部最大值的概率。

### 2.2 求 $\Pr[X_i=1]$

固定位置 $i$。位置 $i$ 是局部最大值 $\iff$ 前缀 $a_1,\dots,a_i$ 中，$a_i$ 是最大的那个。

**引理 2**：对均匀随机的排列，$\Pr[X_i=1]=\dfrac1i$。

**证明**：数一数满足条件的排列。只看前 $i$ 个位置上的**相对大小顺序**：在全部 $n!$ 个排列中，前 $i$ 个位置的 $i$ 个值的相对次序（共 $i!$ 种）是等可能的，每种恰好出现 $n!/i!$ 次。在 $i!$ 种相对次序中，最大值落在第 $i$ 个位置的数目是：先固定「第 $i$ 位是最大」，剩下 $i-1$ 个值在前 $i-1$ 个位置任意排列，共 $(i-1)!$ 种。于是
$$
\Pr[X_i=1]=\frac{(i-1)!}{i!}=\frac1i.
$$
$\square$

（顺带检查 $i=1$：概率 $1$，第一个数总是局部最大值，与直觉一致。）

于是
$$
\mathbb E[X]=\sum_{i=1}^n\frac1i=H_n,
$$
即第 $n$ 个**调和数**。

### 2.3 小 $n$：直接累加

$H_n$ 的定义式就是 $O(n)$ 的循环。$n\le 10^6$ 时直接算即可。

浮点误差要注意：朴素的从左到右累加 $10^6$ 项，误差上界约 $n\varepsilon H_n$。用 64 位 `double`（$\varepsilon\approx2.2\times10^{-16}$）时约 $3\times10^{-9}$，与要求的 $10^{-8}$ 同一量级，不够稳；改用 `long double`（常见平台上 $\varepsilon\approx1.1\times10^{-19}$）后误差约 $10^{-12}$，安全。累加顺序从小到大也最有利于精度（大数加小数不丢位）。

### 2.4 大 $n$：调和数的渐近展开

**引理 3（Euler–Maclaurin 展开）**：当 $n\to\infty$ 时
$$
H_n=\ln n+\gamma+\frac1{2n}-\frac1{12n^2}+\frac1{120n^4}+O\!\left(\frac1{n^6}\right),
$$
其中 $\gamma=0.5772156649015328606\ldots$ 是 Euler–Mascheroni 常数。

**证明**：先说明 $\gamma$ 存在。令 $b_n=H_n-\ln n$。由 $\ln(1+t)>t/(1+t)$（取 $t=\frac1{n-1}$）得
$$
b_n-b_{n-1}=\frac1n-\ln\frac{n}{n-1}<0,
$$
故 $\{b_n\}$ 单调递减；又
$$
b_n=H_n-\ln n=\sum_{k=1}^{n}\Bigl(\frac1k-\int_k^{k+1}\frac{dx}{x}\Bigr)+\int_n^{n+1}\frac{dx}{x}>0,
$$
（每项 $\frac1k-\int_k^{k+1}\frac{dx}{x}\ge0$，因为 $1/x$ 递减），故有下界。单调有界，极限 $\gamma$ 存在。

再对 $f(x)=1/x$ 在 $[1,n]$ 上用 Euler–Maclaurin 求和公式（标准定理）
$$
\sum_{k=1}^n f(k)=\int_1^n f(x)\,dx+\frac{f(1)+f(n)}2+\sum_{j=1}^{m}\frac{B_{2j}}{(2j)!}\bigl(f^{(2j-1)}(n)-f^{(2j-1)}(1)\bigr)+R,
$$
其中 $B_2=\frac16,B_4=-\frac1{30},B_6=\frac1{42},\dots$。代入 $f(x)=1/x$，$f^{(2j-1)}(x)=-(2j-1)!/x^{2j}$，并用 $\gamma=\lim_{N\to\infty}(H_N-\ln N)$ 吸收常数项，即得
$$
H_n=\ln n+\gamma+\frac1{2n}-\frac1{12n^2}+\frac1{120n^4}-\frac1{252n^6}+\cdots.
$$
余项 $R$ 的绝对值不超过第一个被略去的项，即截断到 $1/(120n^4)$ 时误差不超过 $1/(252n^6)$。$\square$

在本题中，只要 $n>10^6$，被截断的项满足
$$
\frac1{252n^6}<\frac1{252\cdot10^{36}}<10^{-38},
$$
相对 $8$ 位小数的要求完全可以忽略。而 $n\le 10^6$ 时公式本身的截断误差也只有
$$
\frac1{120n^4}\le\frac1{120\cdot10^{24}}\approx8\times10^{-27},
$$
其实也够用；我们保守地在小 $n$ 用直接求和，既简单又不损精度。

### 2.5 精度小结

- 小 $n$ 直接加：用 `long double`。
- 大 $n$ 用公式：只涉及 $\log$ 与几次除法，`long double` 的 `logl` 足以保证 $8$ 位小数。
- 输出用 `fixed` + `setprecision(8)`，标准四舍五入。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 只问期望，且「个数」可以拆成每个位置一个 $0/1$ 指示变量。
- **classic problem solution** — 期望线性性：$\mathbb E[X]=\sum_i\Pr[X_i=1]$，无需独立性。
- **explore its property** — 前缀最大值落在第 $i$ 个位置的概率是 $1/i$（对称性），故答案恰为 $H_n$。
- **classic problem solution** — 小 $n$ 直接累加，大 $n$ 用 Euler–Maclaurin 渐近式。

---

## 4. 真实解法（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `long long` | 排列长度，$<2^{31}$ | 读入 | 规模 |
| `ans` | `long double` | 累加中的 $H_n$ | 0 | answer |
| `gam` | `long double`（常量） | Euler–Mascheroni 常数 | $0.5772156649\ldots$ | gamma |
| `x` | `long double` | 公式里的 $n$ | — | 避免整数除法 |

### 4.2 算法步骤

1. 读入 $n$。
2. 若 $n\le 10^6$：令 `ans=0`，对 $i=1\ldots n$ 执行 `ans += 1.0L/i`。
3. 否则：令 `x=(long double)n`，
   $$
   \texttt{ans}=\log x+\gamma+\frac1{2x}-\frac1{12x^2}+\frac1{120x^4}.
   $$
4. 以 `fixed`、`setprecision(8)` 输出 `ans`。

### 4.3 复杂度分析

- **时间**：$O(\min(n,10^6))$，常数很小。
- **空间**：$O(1)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1943 Local Maxima
// https://www.luogu.com.cn/problem/P1943
#include<bits/stdc++.h>
using namespace std;

constexpr long double gam{0.577215664901532860606512090082402431L};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	long long n;
	cin>>n;

	long double ans{0};
	if(n<=1000000)
	{
		for(long long i=1;i<=n;++i)
			ans+=1.0L/i;
	}
	else
	{
		long double x{(long double)n};
		ans=logl(x)+gam+1.0L/(2*x)-1.0L/(12*x*x)+1.0L/(120*x*x*x*x);
	}

	cout<<fixed<<setprecision(8)<<ans<<'\n';
	return 0;
}
```
