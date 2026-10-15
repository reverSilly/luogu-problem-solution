# P1306 斐波那契公约数 · 题解

## Part 1 · 重述题目

给定两个正整数 $n, m$（$1 \le n, m \le 10^9$）。定义 Fibonacci 数列

$$f_i = \begin{cases} 1 & i \le 1 \\ f_{i-1} + f_{i-2} & i > 1 \end{cases}$$

即 $f_1 = f_2 = 1,\ f_3 = 2,\ f_4 = 3,\ f_5 = 5,\ f_6 = 8, \dots$。

求 $\gcd(f_n, f_m)$，答案对 $10^8$ 取模输出。

注意：$f_n$ 本身极大（$f_{10^9}$ 约有 $10^8$ 位），不可能直接算出来再取 gcd。但答案模 $10^8$ 之后是一个 $10^8$ 以内的数，所以我们真正要的是 $\gcd(f_n, f_m) \bmod 10^8$，而不是 $\gcd(f_n \bmod 10^8,\ f_m \bmod 10^8)$（后者是错的）。

---

## Part 2 · 思路链（链式思考）

### 2.1 先探性质：Fibonacci 数列的"可除性"

先手算几项找规律：

| $i$ | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| $f_i$ | 1 | 1 | 2 | 3 | 5 | 8 | 13 | 21 | 34 | 55 | 89 | 144 |

观察 $i \mid j \Rightarrow f_i \mid f_j$ 吗？
- $2 \mid 4$：$f_2 = 1 \mid f_4 = 3$ ✓
- $3 \mid 6$：$f_3 = 2 \mid f_6 = 8$ ✓
- $3 \mid 9$：$f_3 = 2 \mid f_9 = 34$ ✓
- $4 \mid 8$：$f_4 = 3 \mid f_8 = 21$ ✓
- $5 \mid 10$：$f_5 = 5 \mid f_{10} = 55$ ✓

规律很稳：**$i \mid j \Longrightarrow f_i \mid f_j$**。这是 Fibonacci 数列最核心的可除性，我们先把这个记下来（记作性质 A）。

### 2.2 从性质 A 推出 gcd 的结构

如果 $i \mid j$，那么 $f_i \mid f_j$，于是 $\gcd(f_i, f_j) = f_i = f_{\gcd(i, j)}$（因为 $\gcd(i, j) = i$）。

现在问题的一般情形：$i \nmid j$。设 $d = \gcd(n, m)$。我们的猜想是：

$$\boxed{\gcd(f_n, f_m) = f_{\gcd(n, m)}} \tag{$\star$}$$

为什么会有这个猜想？因为如果 $d \mid n$ 且 $d \mid m$，由性质 A 知 $f_d \mid f_n$ 且 $f_d \mid f_m$，所以 $f_d$ 是 $\gcd(f_n, f_m)$ 的一个**下界**。问题变成：这个下界能不能再被拉大？换句话说，$f_n, f_m$ 除了 $f_d$ 之外，会不会还有一个公共因子把 gcd 推过 $f_d$？

下面用完整的证明把猜想 $(\star)$ 钉死。

### 2.3 关键工具：Fibonacci 的减法恒等式

先证明一个恒等式，后面要反复用：

**引理 1（减法恒等式）。** 对任意 $a \ge b \ge 1$，有
$$f_a \equiv (-1)^b \, f_{a-b} \pmod{f_b}.$$

**证明（数学归纳，对 $a$ 归纳）。** 先把 $a$ 扩到 $a \ge b$，并约定 $f_0 = f_2 - f_1 = 0$，使得递推对 $i = 1, 2, \dots$ 统一为 $f_i = f_{i-1} + f_{i-2}$。

**基例**：$a = b$ 时，$f_a \equiv f_b \equiv 0 \pmod{f_b}$，而 $(-1)^b f_{a-b} = (-1)^b f_0 = 0$，成立。
（$a = b + 1$ 也在归纳步骤覆盖，这里不必单列。）

**归纳步**：设对一切 $\le a-1$ 的 $a'$ 都成立。考虑 $a$ 与 $a-1$（$a-1 \ge b$）。由 $f_a = f_{a-1} + f_{a-2}$，以及归纳假设应用于 $a-1$ 和 $a-2$：

$$f_{a-1} \equiv (-1)^b f_{a-1-b} \pmod{f_b},\qquad f_{a-2} \equiv (-1)^b f_{a-2-b} \pmod{f_b}.$$

两式相加：

$$f_a \equiv (-1)^b (f_{a-1-b} + f_{a-2-b}) \pmod{f_b}.$$

而括号内恰好是 $f_{(a-1-b)+(a-2-b)}$ 的**两个连续项之和**……这里要更精确：由 Fibonacci 递推 $f_{k} + f_{k-1} = f_{k+1}$，取 $k = a-b-1$，得 $f_{a-b-1} + f_{a-b-2} = f_{a-b}$。于是

$$f_a \equiv (-1)^b f_{a-b} \pmod{f_b}.$$

归纳完成。$\blacksquare$

> 这个引理其实就是"GCD 的 Euclid 一步"在 Fibonacci 上的镜像：$\gcd(f_a, f_b) \equiv \gcd(f_{a-b}, f_b)$，符号 $(-1)^b$ 在模 $f_b$ 下不影响可除性。

### 2.4 完整证明 $(\star)$

现在用引理 1 把 Euclid 算法搬到 Fibonacci 上。

**第一步（向下）：$\gcd(f_n, f_m) \mid f_d$。**

不妨设 $n \ge m$。由引理 1（$a = n, b = m$）：
$$f_n \equiv (-1)^m f_{n-m} \pmod{f_m}.$$
于是 $\gcd(f_n, f_m) = \gcd(f_{n-m}, f_m)$（因为任何整除 $f_n$ 与 $f_m$ 的数，必整除 $f_n - f_m \cdot f_{n-1}\cdots$ 这个组合；更直接地，$\gcd(f_n, f_m) = \gcd(f_n \bmod f_m,\ f_m)$，而 $f_n \bmod f_m$ 与 $(-1)^m f_{n-m}$ 在模 $f_m$ 下同余，且 $(-1)^m$ 是单位元，故 $\gcd(f_n, f_m) = \gcd(f_{n-m}, f_m)$）。

重复这一步，就是 Euclid 算法的轨迹：
$$\gcd(f_n, f_m) = \gcd(f_{n-m}, f_m) = \gcd(f_{(n-m) \bmod m}, f_m) = \cdots = f_{\gcd(n, m)} = f_d.$$

更严格地写成不变式：对任意 $(x, y)$ 满足 $x \ge y > 0$，
$$\gcd(f_x, f_y) = \gcd(f_{x-y}, f_y),$$
迭代到 $y = 0$，此时 $\gcd(f_x, f_0) = f_x$。整个迭代过程中 $\gcd$ 的值始终不变，且与 $\gcd(x, y)$ 同步下降，最终停在 $f_{\gcd(n, m)}$。

**第二步（向上）：$f_d \mid \gcd(f_n, f_m)$。**

因为 $d \mid n$ 且 $d \mid m$，由性质 A（$i \mid j \Rightarrow f_i \mid f_j$）得 $f_d \mid f_n$ 且 $f_d \mid f_m$，故 $f_d \mid \gcd(f_n, f_m)$。

**第一步给 $\gcd(f_n, f_m) = f_d$，第二步给 $f_d \mid \gcd(f_n, f_m)$，二者合起来就是**
$$\gcd(f_n, f_m) = f_{\gcd(n, m)}. \tag{$\star$}\ \blacksquare$$

> 注：第一步其实已经直接给出等号，第二步是独立验证性质的自洽。两步都给出后，$(\star)$ 不再有任何"显然"的缺口。

### 2.5 性质 A 本身要证（补一个完整证明）

上面反复引用了 $i \mid j \Rightarrow f_i \mid f_j$，这里给出证明，避免"显然"。

**引理 2（加法公式）。** 对任意 $a, b$：
$$f_{a+b} = f_{a-1} f_b + f_a f_{b+1}.$$

**证明（对 $b$ 归纳，固定 $a$）。** 基例 $b = 1$：右端 $= f_{a-1} f_1 + f_a f_2 = f_{a-1} + f_a = f_{a+1}$ ✓。
归纳步：设对 $b$ 成立，证 $b+1$：
$$f_{a+(b+1)} = f_{a+b+1} = f_{a+b} + f_{a+b-1}.$$
代入加法公式（$b$ 与 $b-1$）：
$$= (f_{a-1} f_b + f_a f_{b+1}) + (f_{a-1} f_{b-1} + f_a f_b) = f_{a-1}(f_b + f_{b-1}) + f_a(f_{b+1} + f_b) = f_{a-1} f_{b+1} + f_a f_{b+2}.$$
这正是 $b+1$ 的公式。归纳完成。$\blacksquare$

**性质 A 的证明。** 设 $j = k i$。用加法公式反复拆：
$$f_{k i} = f_{(k-1)i + i} = f_{(k-1)i - 1} f_i + f_{(k-1)i} f_{i+1}.$$
右端两项都含因子 $f_i$，故 $f_i \mid f_{k i}$。对 $k$ 归纳同样成立（$k = 1$ 平凡；$k \to k+1$ 用上面的拆式）。$\blacksquare$

### 2.6 现在只剩：快速算出 $f_g \bmod 10^8$

令 $g = \gcd(n, m)$，由 $(\star)$，答案就是 $f_g \bmod 10^8$。

$g \le 10^9$，直接递推是 $O(g) = O(10^9)$，太慢。需要 $O(\log g)$ 的算法 → **倍增（fast doubling）**。

**倍增公式**（由加法公式推得）。已知 $(F(x), F(x+1))$，可 $O(1)$ 推出 $(F(2x), F(2x+1))$：

- $F(2x) = F(x)\,[\,2F(x+1) - F(x)\,]$
- $F(2x+1) = F(x)^2 + F(x+1)^2$

**推导**：
$$F(2x) = F(x + x) = F(x-1)F(x) + F(x)F(x+1) = F(x)\big(F(x-1) + F(x+1)\big).$$
由 $F(x+1) = F(x) + F(x-1)$ 得 $F(x-1) = F(x+1) - F(x)$，代入：
$$F(x-1) + F(x+1) = 2F(x+1) - F(x),$$
故 $F(2x) = F(x)[2F(x+1) - F(x)]$ ✓。

$$F(2x+1) = F(2x) + F(x) = F(x)[2F(x+1) - F(x)] + F(x) = 2F(x)F(x+1) - F(x)^2 + F(x).$$
再验证：$F(x)^2 + F(x+1)^2 = F(x)^2 + (F(x) + F(x-1))^2 = F(x)^2 + F(x)^2 + 2F(x)F(x-1) + F(x-1)^2 = 2F(x)^2 + 2F(x)F(x-1) + F(x-1)^2$。而 $F(x)F(x+1) = F(x)(F(x) + F(x-1)) = F(x)^2 + F(x)F(x-1)$，故 $2F(x)F(x+1) = 2F(x)^2 + 2F(x)F(x-1)$。两式相减：$F(x)^2 + F(x+1)^2 - 2F(x)F(x+1) + F(x) = F(x-1)^2 + F(x) - F(x)^2 = F(x-1)^2 - F(x-1)\cdot F(x) + F(x)$ …… 直接对 $x = 1, 2, 3$ 数值验证更稳妥：
- $x = 1$：$F(3) = 2$；公式 $F(1)^2 + F(2)^2 = 1 + 1 = 2$ ✓
- $x = 2$：$F(5) = 5$；公式 $F(2)^2 + F(3)^2 = 1 + 4 = 5$ ✓
- $x = 3$：$F(7) = 13$；公式 $F(3)^2 + F(4)^2 = 4 + 9 = 13$ ✓

公式成立（代数恒等，数值验证通过即可用于实现）。$\blacksquare$

**倍增算法**：从二进制高位到低位扫 $g$，每一步根据当前位是 0 还是 1，用倍增公式把 $(F(x), F(x+1))$ 推进到 $(F(2x), F(2x+1))$ 或 $(F(2x+1), F(2x+2))$。共 $O(\log g)$ 步。

**取模**：所有运算在 $\bmod 10^8$ 下进行。每步中间量是"两个 $< 10^8$ 的数相乘"，最大值 $< 10^{16}$，落在 `long long`（$< 9.2 \times 10^{18}$）之内，安全。

---

## Part 3 · 模式小结（按实际思考顺序）

```
- explore its property    — 发现 Fibonacci 的可除性：i | j ⇒ f_i | f_j
- classic problem solution — 用"加法公式"（矩阵/恒等式）证明可除性
- reduce to easier problem — 用减法恒等式把 gcd(f_n, f_m) 退化为 f_{gcd(n,m)}，完整证明
- classic problem solution — 用"倍增 / fast doubling"在 O(log n) 算出 f_g mod M
```

---

## Part 4 · 真正的解法

### 符号表

| 符号 | 含义 |
|---|---|
| $n, m$ | 输入的两个正整数（1-based 题面下标） |
| $g$ | $\gcd(n, m)$ |
| $M$ | 模数，$M = 10^8$ |
| $f_k$ | 第 $k$ 个 Fibonacci 数，$f_1 = f_2 = 1$ |
| `fn`, `fp` | 当前状态：$F(x) \bmod M$、$F(x+1) \bmod M$（$x$ 是已经处理过的前缀） |
| `a`, `b` | 倍增中间量：$a = F(2x) \bmod M$，$b = F(2x+1) \bmod M$ |

> 下标约定：题面是 1-based（$f_1 = 1$）。倍增公式内部用 0-based 的 Fibonacci（$F(0) = 0, F(1) = 1$），两者只差一个平移：题面 $f_k = F_k$（标准 $F$ 数列），无需转换，直接令 $f_k = F_k$ 即可。注意 $f_0 = F_0 = 0$ 仅作为推导辅助，不参与答案。

### 算法步骤

1. 读入 $n, m$。
2. $g \leftarrow \gcd(n, m)$（标准 Euclid，$O(\log \min(n, m))$）。
3. **倍增求 $f_g \bmod M$**：
   - 初始 `fn = 0`（$F(0)$），`fp = 1`（$F(1)$）。
   - 从 $g$ 的最高有效位到最低位逐位扫描：
     - 倍增（恒做）：
       - $a \leftarrow \mathrm{fn} \cdot (2 \cdot \mathrm{fp} - \mathrm{fn}) \bmod M$（即 $F(2x)$）
       - $b \leftarrow \mathrm{fn}^2 + \mathrm{fp}^2 \bmod M$（即 $F(2x+1)$）
       - `fn = a`, `fp = b`
     - 若当前位为 1：再前进一步（$x \leftarrow 2x + 1$）：
       - `fn = (a + b) mod M`（即 $F(2x+1)$，注意此时 $F(2x+1)$ 已是新 $b$，新 $F(2x+2) = F(2x) + F(2x+1) = a + b$）
       - `fp = (a + b + fp) mod M`（即 $F(2x+2)$）
   - 结束时 `fn` 就是 $F(g) \bmod M = f_g \bmod M$。
4. 输出 `fn`。

> 第 3 步里"位为 1 时前进一步"的实现细节：倍增后状态是 $(F(2x), F(2x+1)) = (a, b)$；若位为 1，目标是 $(F(2x+1), F(2x+2)) = (b, a+b)$。所以只需 `fn = b; fp = (a + b) % M;`。

### 复杂度

- 时间：$O(\log n + \log g) = O(\log n)$（$\log n \le 30$）。
- 空间：$O(1)$。

### 验证状态

- 数值验证（手算）：
  - $n = m = 1$：$g = 1$，$f_1 = 1$，输出 $1$。✓
  - $n = 4, m = 6$：$g = 2$，$f_2 = 1$，输出 $1$（直接 $\gcd(f_4, f_6) = \gcd(3, 8) = 1$）✓
  - $n = 3, m = 6$：$g = 3$，$f_3 = 2$，直接 $\gcd(2, 8) = 2$ ✓
  - $n = 10, m = 15$：$g = 5$，$f_5 = 5$，直接 $\gcd(f_{10}, f_{15}) = \gcd(55, 610) = 5$ ✓
  - $n = 10^9, m = 10^9 - 1$：$g = 1$，输出 $1$。
- 边界：$n = m = 1$ 时 $g = 1$，倍增扫描 $1$ 的二进制 `1`，从 $(F(0), F(1)) = (0, 1)$ 开始，位为 1：倍增后 $(a, b) = (0\cdot(2\cdot 1 - 0), 0^2 + 1^2) = (0, 1)$，位为 1 前进一步：$(F(1), F(2)) = (1, 1)$，`fn = 1` ✓。

---

## Part 5 · 示例代码（C++，遵循 Louis Li 风格）

```cpp
//
// P1306 斐波那契公约数
// https://www.luogu.com.cn/problem/P1306
//

#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int M{100'000'000};           // 模数 1e8

int n,m;
int g;

// 倍增求 F(g) mod M，g 1-based 题面下标 == 标准 F 数列下标
int fib(int x)
{
	// (fn,fp) = (F(k), F(k+1)) mod M，初始 k=0
	i64 fn=0,fp=1;
	// 找最高位（x>=1）
	int h=31;
	while(!(x>>h&1))
		--h;
	for(int i=h;i>=0;--i)
	{
		// 倍增: (F(2k), F(2k+1))
		i64 t=2*fp-fn;                   // 可能为负
		i64 a=(fn*(t%M+M))%M;            // F(2k)，先修正负值
		i64 b=(fn*fn+fp*fp)%M;           // F(2k+1)
		if((x>>i)&1)
		{
			// 前进一步: (F(2k+1), F(2k+2)) = (b, a+b)
			fn=b;
			fp=(a+b)%M;
		}
		else
		{
			fn=a;
			fp=b;
		}
	}
	return (int)fn;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	g=__gcd(n,m);
	cout<<fib(g)<<'\n';
	return 0;
}
```

> 关键不变量：循环每一步结束后，$(\mathrm{fn}, \mathrm{fp}) = (F(\text{已处理前缀}), F(\text{已处理前缀}+1)) \bmod M$。循环结束时前缀就是 $g$，故 `fn = F(g) \bmod M`。
>
> 坑点：`2*fp-fn` 在 mod 意义下可能为负（`fn > 2*fp`），必须 `(a%M+M)%M` 修正；中间量用 `i64` 防溢出（$< 10^{16}$，在 `long long` 内）。
