# P1226 【模板】快速幂 题解

## Part 1: 重述问题

给定三个整数 $a, b, p$，求 $a^b \bmod p$。

**约束：**
- $0 \le a, b < 2^{31}$
- $a + b > 0$（即 $a$ 和 $b$ 不同时为 0，排除了 $0^0$ 的歧义）
- $2 \le p < 2^{31}$

**输出格式：** `a^b mod p=s`，其中 $a, b, p$ 为输入值，$s$ 为计算结果。

**样例：** $a=2, b=10, p=9$，$2^{10}=1024$，$1024 \bmod 9 = 7$，输出 `2^10 mod 9=7`。

---

## Part 2: 思考过程

### 第一步：探索性质

我们要计算 $a^b \bmod p$。最朴素的方法是：

```
result = 1
for i in 1 to b:
    result = result * a
print result mod p
```

这需要 $O(b)$ 次乘法，而 $b < 2^{31} \approx 21$ 亿，太慢了。

关键问题：**能否用更少的乘法算出 $a^b$？**

注意到 $a^b$ 中的指数 $b$ 可以写成二进制。例如 $b = 10 = (1010)_2 = 2^3 + 2^1$，所以：

$$a^{10} = a^{8} \cdot a^{2}$$

更一般地，设 $b$ 的二进制表示为 $b = \sum_{i=0}^{\lfloor \log_2 b \rfloor} \varepsilon_i \cdot 2^i$，其中 $\varepsilon_i \in \{0, 1\}$ 是第 $i$ 位，则：

$$a^b = a^{\sum \varepsilon_i \cdot 2^i} = \prod_{i: \varepsilon_i = 1} a^{2^i}$$

所以，我们只需要知道每个 $a^{2^i} \bmod p$（$i = 0, 1, \ldots$），然后把对应位为 1 的那些相乘即可。

**核心观察：$a^{2^i}$ 之间有关系。**

$$a^{2^{i+1}} = (a^{2^i})^2$$

这意味着 $a^{2^0} = a$，$a^{2^1} = a^2$，$a^{2^2} = a^4$，……，$a^{2^{i+1}} = (a^{2^i})^2$。

所以如果我们在计算过程中维护 $a^{2^i} \bmod p$，每往高一位走，只需做一次平方。这样我们最多需要 $\lfloor \log_2 b \rfloor + 1 \le 31$ 次平方 + 31 次乘法（最多），总共约 62 次模乘。

### 第二步：考虑更简单的版本

先不考虑 $\bmod p$，就纯算 $a^b$。那么上面说的：

```
ans = 1
for i = 0 to 30:
    if b 的第 i 位为 1:
        ans = ans * a^(2^i)
    a^(2^(i+1)) = (a^(2^i))^2
```

这就对了。加上 $\bmod p$ 后，由模乘的分配律 $(xy) \bmod p = ((x \bmod p) \cdot (y \bmod p)) \bmod p$，每一步乘法都取模即可。

但这里有一个**溢出问题**。$a, p < 2^{31}$，所以 $a \bmod p < 2^{31}$，两个这样的数相乘最大约 $(2^{31})^2 = 2^{62} \approx 4.6 \times 10^{18}$，超过 `int`（最大 $\approx 2.1 \times 10^9$）和 `long long` 的安全范围吗？

`long long` 最大约 $9.2 \times 10^{18}$，$2^{62} \approx 4.6 \times 10^{18}$，所以 `long long` 可以装下。但为了安全，我们用 `u64 = unsigned long long`，最大约 $1.8 \times 10^{19}$，绝对够。

### 第三步：经典算法

这就是经典的**快速幂（二进制取幂）**算法，核心思想是：

> 利用指数的二进制分解，把 $O(b)$ 次乘法压缩到 $O(\log b)$ 次。

伪代码：

```
function qpow(a, b, p):
    ans = 1
    a = a mod p
    while b > 0:
        if b & 1:
            ans = (ans * a) mod p
        a = (a * a) mod p
        b = b >> 1
    return ans
```

### 第四步：证明正确性

**定理：** 上述算法返回 $a^b \bmod p$。

**证明：**

用循环不变量（loop invariant）来证明。设原始输入为 $a_0, b_0$。算法中 $a$ 变量初始为 $a_0 \bmod p$，$b$ 变量初始为 $b_0$，$ans$ 初始为 $1$。

**循环不变量：** 在第 $k$ 次迭代开始前（$k = 0, 1, 2, \ldots$），以下三条同时成立：
1. $b = \lfloor b_0 / 2^k \rfloor$（即 $b_0$ 右移 $k$ 位）
2. $a = a_0^{2^k} \bmod p$
3. $ans \equiv a_0^{b_0 \bmod 2^k} \pmod{p}$

即 $ans$ 已经累乘了 $b_0$ 低 $k$ 位中所有为 1 的位对应的 $a_0^{2^i}$。

**基础：** $k = 0$。$b = b_0$，$a = a_0^{1} \bmod p = a_0 \bmod p$，$ans = 1 = a_0^0 = a_0^{b_0 \bmod 1}$。$\checkmark$

**归纳：** 假设第 $k$ 次迭代前不变量成立。执行迭代：

- 若 $b$ 的最低位为 1（即 $b_0$ 的第 $k$ 位为 1）：$ans \leftarrow ans \cdot a \bmod p = a_0^{b_0 \bmod 2^k} \cdot a_0^{2^k} \bmod p = a_0^{b_0 \bmod 2^k + 2^k} \bmod p = a_0^{(b_0 \bmod 2^{k+1})} \bmod p$
- 若 $b$ 的最低位为 0：$ans$ 不变，仍为 $a_0^{b_0 \bmod 2^k}$。而 $b_0 \bmod 2^{k+1} = b_0 \bmod 2^k + 0 \cdot 2^k = b_0 \bmod 2^k$（因为第 $k$ 位为 0）。$\checkmark$

然后 $a \leftarrow a^2 \bmod p = (a_0^{2^k})^2 \bmod p = a_0^{2^{k+1}} \bmod p$，$b \leftarrow b \gg 1 = \lfloor b_0/2^{k+1} \rfloor$。

不变量在第 $k+1$ 次迭代前成立。$\checkmark$

**终止：** 当 $b = 0$ 时，$k = \lfloor \log_2 b_0 \rfloor + 1$，$b_0 \bmod 2^k = b_0$（因为 $2^k > b_0$），所以 $ans = a_0^{b_0} \bmod p$。$\blacksquare$

### 第五步：复杂度分析

$b < 2^{31}$，所以循环最多 31 次。每次循环做最多 2 次模乘（一次平方 + 一次可能的乘法），共约 62 次模乘，时间复杂度 $O(\log b)$。

空间复杂度 $O(1)$。

### 第六步：边界情况

- $a = 0, b \geq 1$：$a_0 \bmod p = 0$，第一次循环若 $b$ 最低位为 1 则 $ans = 0$，之后 $a = 0$，$ans$ 始终为 0。返回 0。正确，因为 $0^b = 0$（$b \geq 1$）。
- $b = 0$：$a + b > 0$ 保证 $a > 0$。$a^0 = 1$，算法中 $ans = 1$，循环不执行，返回 1。正确。
- $a \geq p$：先 $a \bmod p$，没问题。

---

## Part 3: 模式总结

```
- explore its property — 指数 b 的二进制分解：a^b = ∏ a^(2^i) (ε_i=1)
- consider easier version — 先不管 mod，纯乘法；再考虑溢出，用 u64
- classic problem solution — 快速幂（二进制取幂）：O(log b) 次模乘
- prove every lemma — 循环不变量证明：ans 累乘低 k 位对应的 a^(2^i)
```

---

## Part 4: 算法描述

### 符号表

| 符号 | 含义 |
|------|------|
| $a, b, p$ | 输入的三个整数 |
| $a_{in}$ | 原始输入的 $a$ 值（用于输出） |
| $ans$ | 累乘结果，初始为 1 |
| $a$ | 当前底数，初始为 $a_{in} \bmod p$，每轮平方 |
| $b$ | 当前指数，初始为输入值，每轮右移 1 位 |

### 算法步骤

1. 读入 $a, b, p$。
2. 设 $ans = 1$，$a = a \bmod p$。
3. 当 $b > 0$ 时循环：
   - 若 $b$ 的最低位为 1（$b \& 1$），则 $ans = (ans \times a) \bmod p$。
   - $a = (a \times a) \bmod p$（平方）。
   - $b = b \gg 1$（右移 1 位）。
4. 输出 `a^b mod p=ans`（其中 $a, b, p$ 为原始输入值）。

### 复杂度

- **时间：** $O(\log b)$，$b < 2^{31}$ 时最多 31 轮循环。
- **空间：** $O(1)$。

### 溢出说明

$a, p < 2^{31}$，故 $a \bmod p < 2^{31}$，两次相乘最大 $(2^{31}-1)^2 \approx 4.6 \times 10^{18}$，用 `unsigned long long`（最大 $2^{64}-1 \approx 1.8 \times 10^{19}$）可安全容纳。

### 验证状态

逻辑已通过与样例手算验证（$2^{10} \bmod 9 = 7$）及边界情况检查（$a=0, b=1 \Rightarrow 0$；$b=0, a=5 \Rightarrow 1$）。

---

## Part 5: C++ 代码

```cpp
#include<bits/stdc++.h>
using namespace std;
using u64=unsigned long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	u64 a,b,p;
	cin>>a>>b>>p;
	u64 b0=b,base=a%p,ans=1;
	for(;b0;b0>>=1,base=base*base%p)
		if(b0&1)
			ans=ans*base%p;
	cout<<a<<'^'<<b<<" mod "<<p<<'='<<ans<<'\n';
	return 0;
}
```

**代码说明：**
- `a, b, p` 保存原始输入值，用于输出。
- `b0` 是 $b$ 的副本，作为循环变量被消耗（右移直到 0）。
- `base` 是当前底数 $a^{2^k} \bmod p$，每轮平方一次。
- `ans` 是累乘结果，仅当 `b0` 当前最低位为 1 时乘以 `base` 并取模。
- 使用 `u64` 防止中间乘积溢出（$(2^{31}-1)^2 \approx 4.6 \times 10^{18}$，在 `u64` 范围内）。
