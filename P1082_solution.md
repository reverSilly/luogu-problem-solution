# P1082 [NOIP 2012 提高组] 同余方程 题解

---

## 1. 重新审题

给定同余方程

$$
ax \equiv 1 \pmod{b}
$$

其中 $2 \le a, b \le 2 \times 10^9$，$a,b$ 均为整数。要求输出**最小正整数解** $x_0$。题目保证一定有解。

"一定有解"意味着 $\gcd(a, b) = 1$。原因：$ax \equiv 1 \pmod{b}$ 等价于存在整数 $y$ 使得 $ax + by = 1$，由 Bézout 定理，该方程有整数解当且仅当 $\gcd(a, b) \mid 1$，即 $\gcd(a, b) = 1$。

---

## 2. 解题思维链（完整推导与证明）

### 2.1 从同余方程到不定方程

$$
ax \equiv 1 \pmod{b} \iff \exists\, y \in \mathbb{Z},\; ax + by = 1.
$$

设 $d = \gcd(a, b)$。

- 若 $d \nmid 1$，无解。题目保证有解 $\Rightarrow d = 1$。
- 问题转化为：求整数对 $(x, y)$ 满足 $ax + by = 1$，且 $x$ 是最小正整数。

### 2.2 扩展欧几里得算法（Extended Euclidean Algorithm）

标准欧几里得算法：

$$
\gcd(a, b) = \gcd(b, a \bmod b).
$$

扩展欧几里得算法在递归计算 $\gcd$ 的同时，逆向回代求出系数。

**引理 2.2.1（扩展欧几里得递归基础）**  
已知 $bx' + (a \bmod b)\, y' = d$，可推导出 $ax + by = d$，其中

$$
a \bmod b = a - \left\lfloor \frac{a}{b} \right\rfloor \cdot b.
$$

代入：

$$
bx' + \bigl(a - \lfloor a/b \rfloor \cdot b\bigr) y' = d
\;\Longrightarrow\;
a y' + b\bigl(x' - \lfloor a/b \rfloor \cdot y'\bigr) = d.
$$

因此令

$$
x = y', \qquad y = x' - \lfloor a/b \rfloor \cdot y'.
$$

递归基：当 $b = 0$ 时，$d = a$，方程 $a x + 0 \cdot y = a$ 的一组解为 $x = 1,\; y = 0$。

**算法 2.2.2（exgcd）**  
输入 $a, b$，输出 $d = \gcd(a,b)$ 及一组特解 $(x, y)$ 满足 $ax + by = d$。

```
exgcd(a, b, &x, &y):
    if b == 0: x = 1, y = 0, return a
    d = exgcd(b, a % b, x, y)
    t = x
    x = y
    y = t - (a / b) * y
    return d
```

调用 `exgcd(a, b, x, y)` 后得到 $ax + by = 1$，$x$ 即为一组特解。

### 2.3 最小正整数解的调整

得到的 $x$ 可能为负或零。通解形式为

$$
x_k = x + k \cdot \frac{b}{d} = x + k \cdot b \quad (\because d = 1).
$$

要得到最小正整数解，需调整到 $(0, b]$ 范围内：

$$
x_0 = \bigl((x \bmod b) + b\bigr) \bmod b.
$$

此运算将 $x$ 映射到 $[0, b-1]$，当结果为 $0$ 时取 $b$（但由 $a x \equiv 1 \pmod{b}$ 可知 $x \not\equiv 0 \pmod{b}$，故结果不为 $0$）。

**证明正确性**：设 $x_0$ 为最小正整数解。若存在更小的正整数解 $x'$，则 $x' - x_0$ 为 $b$ 的倍数，矛盾。

### 2.4 复杂度分析

- 时间复杂度：$O(\log \min(a, b))$，即扩展欧几里得算法的递归深度。
- 空间复杂度：$O(\log \min(a, b))$（递归栈）或 $O(1)$（迭代实现）。

---

## 3. 模式提炼

- **识别特征**：形如 $ax \equiv 1 \pmod{b}$ 的线性同余方程，求最小正整数解。
- **关键洞察**：同余方程等价于不定方程 $ax + by = 1$，由 Bézout 定理知有解条件为 $\gcd(a, b) = 1$。
- **通用解法**：扩展欧几里得算法求特解 $x$，再取模调整至 $[1, b]$。
- **常见错误**：
  1. 未处理 $x < 0$ 的情况直接输出。
  2. `((x % b) + b) % b` 中的 `%` 在 C++ 中对负数取模结果为负或零。

---

## 4. 最终解法（含符号表）

### 符号表

| 符号 | 含义 |
|------|------|
| $a, b$ | 输入整数，$2 \le a,b \le 2\times 10^9$ |
| $x, y$ | 不定方程 $ax + by = 1$ 的一组整数解 |
| $d$ | $\gcd(a, b)$，题目保证 $d = 1$ |
| $x_0$ | 最小正整数解 |
| $k$ | 通解参数，$x = x_0 + k \cdot b$ |

### 解法步骤

1. 调用扩展欧几里得算法 `exgcd(a, b)`，返回 $d$ 及特解 $x, y$。
2. 调整 $x$ 至最小正整数：$x_0 = ((x \bmod b) + b) \bmod b$。
3. 输出 $x_0$。

---

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64 = long long;

void exgcd(i64 a, i64 b, i64 &x, i64 &y)
{
	if (b == 0)
	{
		x = 1;
		y = 0;
		return;
	}
	exgcd(b, a % b, y, x);
	y -= a / b * x;
}

i64 inv(i64 a, i64 b)
{
	i64 x, y;
	exgcd(a, b, x, y);
	return (x % b + b) % b;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	i64 a, b;
	cin >> a >> b;
	cout << inv(a, b) << '\n';

	return 0;
}
```

### 关键点说明

- **类型使用 `i64`（`long long`）**：$a,b \le 2\times 10^9$，乘法可能溢出 `int`，必须用 64 位整数。
- **`exgcd` 的 `y -= a / b * x`**：对应推导中 $y = x' - \lfloor a/b \rfloor \cdot y'$。递归时交换 `x, y` 的位置（`exgcd(b, a % b, y, x)`），使返回时 $x$ 自动成为上一层的 $y'$，$y$ 为 $x'$。
- **`(x % b + b) % b`**：C++ 中 `%` 对负数结果为负，加 `b` 再取模保证结果在 $[0, b-1]$。
- 题目保证有解，无需特判无解情形。
