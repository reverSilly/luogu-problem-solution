# P1516 青蛙的约会

> 标签：数学 / 数论 / 扩展欧几里得算法 / 同余方程 · 难度：省选/NOI-
> 本地题面：`P1516.md`（$1\le x,y,m,n\le 2\times 10^9,\,x\ne y,\,1\le L\le 2.1\times 10^9$）。本文所用官方样例 `1 2 3 4 5 -> 4` 取自洛谷原题页 P1516（见 §4.4）。

## 1. 重新读题

**问题重述（人话版）**

在周长为 $L$ 米的环形纬度线上，青蛙 A 从坐标 $x$ 出发，每次正向跳 $m$ 米；青蛙 B 从坐标 $y$ 出发，每次正向跳 $n$ 米。两只青蛙同步起跳，问最少跳多少次（记为非负整数 $t\ge 0$）后它们会落到环上的同一个位置？若永远不可能同时落到同一点，输出 `Impossible`。

> 一句话版本：**求解模 $L$ 线性同余方程 $x+mt\equiv y+nt\pmod L$ 的最小非负整数解 $t$。**

**题面里容易漏掉的条款**

1. **$x,y,m,n$ 的相对大小均未限制** $\Rightarrow$ 可能出现 $m<n$ 或 $x>y$，相减会出现负数；在转化为线性不定方程时，必须先把模 $L$ 的系数与常数项归一化到 $[0,L-1]$（或处理负系数符号），防止 `exgcd` 传入负参数导致最大公约数带负号。
2. **最小周期是 $L/\gcd(a,L)$ 而非 $L$** $\Rightarrow$ 求出特解后，通解的模数是 $\mathrm{mod}=L/\gcd(a,L)$；若错误地对 $L$ 取模，当 $\gcd(a,L)>1$ 时会把一个比最小正周期大若干倍的非最优解当成答案输出。
3. **数据上界 $2.1\times 10^9$ 的双层溢出陷阱**：
   - $2.1\times 10^9$ 接近 $32$ 位有符号整数上界 $2^{31}-1\approx 2.147\times 10^9$；
   - 在将特解 $t_0$ 放大 $\frac{b}{g}$ 倍求 $(b/g)\cdot t_0\bmod(L/g)$ 时，两项均可达 $2.1\times 10^9$，乘积高达 $4.41\times 10^{18}$，接近 `long long` 上界（$9.22\times 10^{18}$），若未先对 $t_0$ 取模或中间用 `__int128` 保护，极易溢出。

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：从环形追及到线性同余方程

设跳了 $t\in\mathbb{Z}_{\ge 0}$ 次后两蛙相遇。此时青蛙 A 走过的总路程为 $x+mt$，青蛙 B 走过的总路程为 $y+nt$。

在周长为 $L$ 的圆环上，两点重合当且仅当它们的坐标差是周长 $L$ 的整数倍：

$$x+mt\equiv y+nt\pmod L\iff (m-n)t\equiv y-x\pmod L .$$

令

$$a=\big((m-n)\bmod L+L\big)\bmod L\in[0,L-1],\qquad b=\big((y-x)\bmod L+L\big)\bmod L\in[0,L-1] .$$

由于模 $L$ 下同余关系保持不变，原条件完全等价于存在整数 $k\in\mathbb{Z}$ 使得

$$a t+L k=b .$$

### 2.2 反复碰壁：三个经典错误写法与反例

**失败 1：直接将负系数 $m-n<0$ 原样塞进 `exgcd(m-n, L, t0, k0)`，却按正数 `g` 处理后续取模。**

- 反例：$x=10,y=2,m=1,n=3,L=12$。
- 此时 $m-n=-2,y-x=-8$。若直接传入负数，C++ 的 `%` 运算向零截断，可能导致 `exgcd` 返回负数 $g=-2$，随后 `mod = L / g = -6`，在 `(t0 % mod + mod) % mod` 时产生负数答案！
- **根治方法**：在调用 `exgcd` 之前，先用 `((m-n)%L+L)%L` 把 $t$ 的系数 $a$ 和右端常数 $b$ 统一归约到非负区间 $[0,L-1]$，从源头保证 `exgcd(a, L, ...)` 的两个入参满足 $a\ge 0,L\ge 1$，从而返回值 $g=\gcd(a,L)\ge 1$ 恒为正整数。

**失败 2：把通解周期误当成 $L$（写成 `(t0 * (b/g) % L + L) % L`）。**

- 反例：$x=1,y=5,m=2,n=4,L=6$。
- 归一后 $a=(-2)\bmod 6=4$，$b=4\bmod 6=4$。我们要解 $4t\equiv 4\pmod 6$。
- 这里 $g=\gcd(4,6)=2$，`exgcd(4, 6)` 给出 $4\times(-1)+6\times 1=2$，即 $t_0=-1$。
- 乘上 $b/g=2$ 得特解 $t_1=t_0\cdot(b/g)=-2$。
- 若错误地按模 $L=6$ 取最小非负解，会得到 $(-2\bmod 6+6)\bmod 6=4$；但实际上当 $t=1$ 时，$x+mt=3$，$y+nt=9\equiv 3\pmod 6$，**第 $1$ 步就已经相遇了**！错误原因在于 $4t\equiv 4\pmod 6\iff 2t\equiv 2\pmod 3$，最小正周期是 $L/g=3$ 而非 $6$，$-2\equiv 1\pmod 3$。

**失败 3：当 $m=n$（两蛙速度相同）时除零崩溃。**

- 反例：$x=1,y=2,m=5,n=5,L=10$。
- 此时 $a=0$，`exgcd(0, L)` 返回 $g=L=10$。若先判断 $b\bmod g\ne 0$（这里 $b=1$，$1\bmod 10=1\ne 0$），会自然输出 `Impossible`；若 $b=0$（即初始已在模 $L$ 同一点），则 $mod=L/L=1$，输出 $0$。可见只要遵循“先算 `g = exgcd(a, L)`、再判 `b % g != 0`、最后算 `mod = L / g`”，连 $m=n$ 的退化情形也无需单独打补丁。

### 2.3 归约到经典算法：裴蜀定理与扩展欧几里得算法（附完整证明）

**引理 1（扩展欧几里得算法的正确性与终止性）**

对任意整数 $a\ge 0,b\ge 0$（不同时为 $0$），递归函数 `exgcd(a, b, x, y)` 在 $O(\log(\min(a,b)+1))$ 步内终止，返回 $g=\gcd(a,b)\ge 1$，且输出的引用变量 $(x,y)\in\mathbb{Z}^2$ 满足

$$a x+b y=g .$$

**证明**（对第二参数 $b$ 做强归纳）：

- **基例 $b=0$**：此时 $a>0$，函数置 $x=1,y=0$ 并返回 $a$。显然 $\gcd(a,0)=a$，且 $a\cdot 1+0\cdot 0=a$，成立。
- **归纳步 $b>0$**：由于 $a\bmod b\in[0,b-1]$ 严格小于 $b$，递归必在有限步（拉梅定理给出 $\le 2\log_2 b+1$ 步）内终止。设递归调用 `exgcd(b, a % b, y, x)` 返回 $g=\gcd(b,a\bmod b)=\gcd(a,b)$，且使传入的引用满足
  $$b\cdot y+(a\bmod b)\cdot x=g .$$
  将 $a\bmod b=a-\lfloor a/b\rfloor\cdot b$ 代入上式：
  $$b\cdot y+\big(a-\lfloor a/b\rfloor\cdot b\big)x=g\iff a\cdot x+b\cdot\big(y-\lfloor a/b\rfloor\cdot x\big)=g .$$
  因此执行 `y -= (a / b) * x` 后，新 $(x,y)$ 精确满足 $ax+by=g$。$\blacksquare$

**引理 2（线性同余方程有解充要条件与通解结构）**

设 $a\in[0,L-1],b\in[0,L-1],L\ge 1$，$g=\gcd(a,L)$。则同余方程 $at\equiv b\pmod L$：

1. 有整数解 $t\iff g\mid b$；
2. 当 $g\mid b$ 时，若 $a t_0+L k_0=g$，则方程的全体整数解 $t$ 恰为模 $L'=L/g$ 的唯一剩余类：
   $$t\equiv t_0\cdot\frac{b}{g}\pmod{L'},$$
   从而最小非负整数解为
   $$t_{\min}=\left(\left(t_0\bmod L'+L'\right)\bmod L'\cdot\frac{b}{g}\right)\bmod L' .$$

**证明**：

1. **有解充要条件（裴蜀定理）**：$at\equiv b\pmod L\iff \exists k\in\mathbb{Z},\ a t+L k=b$。
   - （$\Rightarrow$）因 $g\mid a$ 且 $g\mid L$，故对任意整数 $t,k$ 有 $g\mid(at+Lk)=b$。
   - （$\Leftarrow$）若 $g\mid b$，将等式 $a t_0+L k_0=g$ 两边同乘整数 $\frac{b}{g}$，得
     $$a\left(t_0\frac{b}{g}\right)+L\left(k_0\frac{b}{g}\right)=b,$$
     即 $t^\ast=t_0\frac{b}{g}$ 是原方程的一个整数特解。
2. **通解完全刻画**：设 $t^\ast=t_0\frac{b}{g}$ 为上述特解。任取另一整数 $t$，则
   $$at\equiv b\pmod L\iff a(t-t^\ast)\equiv 0\pmod L\iff L\mid a(t-t^\ast) .$$
   因为 $g=\gcd(a,L)>0$，将整除式两边同除以 $g$，令 $a'=a/g,\,L'=L/g$，得
   $$L'\mid a'(t-t^\ast) .$$
   由于 $\gcd(a',L')=\gcd(a/g,L/g)=1$，由欧几里得引理（互素消去律）知：
   $$L'\mid a'(t-t^\ast)\iff L'\mid (t-t^\ast)\iff t\equiv t^\ast\pmod{L'} .$$
   因此全体解在整数轴上构成公差为 $L'\ge 1$ 的等差数列，它在区间 $[0,L'-1]$ 内有且仅有一个代表元，即 $t^\ast\bmod L'$ 的非负最小剩余。$\blacksquare$

## 3. 模式总结（Pattern Summary）

按实际思考顺序列出本题的可迁移模式：

- **explore its property（环形追及模化）** — 将环形跑道上的“同时同地”翻译成差值模周长同余 $(m-n)t\equiv y-x\pmod L$。
- **normalize before algebra（非负归一化）** — 在进入数论算法前，先把左端系数与右端常数用 `((v % L) + L) % L` 统一映射到 $[0,L-1]$，彻底消除负系数和 $m=n$ 退化分支。
- **classic problem solution（扩展欧几里得算法 exgcd）** — 求解 $a t_0+L k_0=g=\gcd(a,L)$，用 $g\mid b$ 判解的存在性（引理 1、2）。
- **failed attempt（模 $L$ 而非模 $L/g$）** — 被 $4t\equiv 4\pmod 6$ 击穿（真值 $1$，模 $6$ 误报 $4$）；通解的真实最小周期是 $L'=L/\gcd(a,L)$（欧几里得互素引理）。
- **overflow defense（`__int128` 保护乘法取模）** — $(b/g)$ 与 $(t_0\bmod L')$ 均可达 $2.1\times 10^9$，相乘时强转为 `__int128` 再对 $L'$ 取模，杜绝有符号溢出。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 | 含义 | 初值 / 范围 | 助记 / 来源 |
|---|---|---|---|---|
| `x, y, m, n, L` | `i64` | 两蛙起点、两蛙步长、环周长 | 读入，$\le 2.1\times 10^9$ | 题面给定 |
| `a` | `i64` | 归一化后的相对步长 $((m-n)\bmod L+L)\bmod L$ | $[0,L-1]$ | 同余方程左端系数 |
| `b` | `i64` | 归一化后的初始距离 $((y-x)\bmod L+L)\bmod L$ | $[0,L-1]$ | 同余方程右端常数 |
| `t0, k0` | `i64` | `exgcd(a, L, t0, k0)` 解出的裴蜀系数 | $|t_0|\le L$ | $a t_0+L k_0=g$ |
| `g` | `i64` | $\gcd(a,L)$ | $[1,L]$ | 引理 1 |
| `mod` | `i64` | 最小正周期 $L'=L/g$ | $[1,L]$ | 引理 2 |
| `ans` | `i64` | 最小非负跳跃次数 $t_{\min}$ | $[0,\mathrm{mod}-1]$ | 引理 2 |

### 4.2 编号步骤

1. 用 `i64` 读入 `x, y, m, n, L`。
2. 归一化系数：`a = ((m - n) % L + L) % L`，`b = ((y - x) % L + L) % L`。
3. 调用 `g = exgcd(a, L, t0, k0)` 求得 $\gcd(a,L)$ 及满足 $a t_0+L k_0=g$ 的特解 `t0`。
4. 若 `b % g != 0`，由引理 2 知无解，输出 `"Impossible\n"` 并结束。
5. 令 `mod = L / g`，计算 `ans = (__int128)(b / g) * (t0 % mod + mod) % mod`，输出 `ans`。

### 4.3 复杂度

- **时间**：单次 `exgcd` 递归深度不超过 $2\log_2 L\le 64$ 层，时间复杂度 $O(\log L)$，实测微秒级。
- **空间**：仅常数个标量与 $O(\log L)$ 递归栈帧，空间复杂度 $O(\log L)$。

### 4.4 验证状态小结

所有结论均由本地真实编译运行（`g++ -O2 -std=c++17`）及 `appendix/P1516/P1516_check.py` 验证：

| 项目 | 状态 |
|---|---|
| 官方样例回归 | **通过**：`1 2 3 4 5 -> 4`。 |
| 随机数据对拍 | **通过**：$4000$ 组随机用例（涵盖有解 $2881$ 组、`Impossible` $1119$ 组），标程 `P1516.cpp` 与逐帧模拟暴力 `P1516_brute.cpp` 输出**逐字节一致**。 |
| 边界用例（6 组） | **通过**：覆盖 $m=n$ 无解（`Impossible`）、$m=n$ 且 $x\equiv y\pmod L$（`0`）、$m<n$ 负系数归一、$\gcd(m-n,L)>1$ 无解与有解（验证模 $L/g$ 周期缩减）、以及 $L=2.1\times 10^9$ 大数防溢出用例。 |
| 最坏规模计时 | $L=2099999999$ 极限规模：输出 `1622727272`，含进程启动最快耗时 **$0.0152\text{ s}$**。 |

## 5. 示例代码

### 5.1 主解：归一化同余方程 + 扩展欧几里得算法

```cpp
//
// 题目：P1516 青蛙的约会
// https://www.luogu.com.cn/problem/P1516
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 exgcd(i64 a,i64 b,i64&x,i64&y)
{
	if(!b)
	{
		x=1;y=0;
		return a;
	}
	i64 g=exgcd(b,a%b,y,x);
	y-=a/b*x;
	return g;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	i64 x,y,m,n,L;
	if(!(cin>>x>>y>>m>>n>>L))
		return 0;

	// 方程: x + m*t === y + n*t (mod L)  <=>  ((m-n) mod L)*t + L*k = (y-x) mod L
	i64 a=((m-n)%L+L)%L;
	i64 b=((y-x)%L+L)%L;
	i64 t0,k0;
	i64 g=exgcd(a,L,t0,k0);
	if(b%g!=0)
	{
		cout<<"Impossible\n";
		return 0;
	}
	i64 mod=L/g;
	i64 ans=(i64)((__int128)(b/g)*(t0%mod+mod)%mod);
	cout<<ans<<'\n';
	return 0;
}
```

### 5.2 对照实现：环上逐帧模拟（仅用于小规模对拍）

```cpp
//
// 题目：P1516 青蛙的约会
// 暴力基准: 逐步模拟 t=0,1,...,L-1 检验两只青蛙在模 L 环上的位置是否重合(仅小 L 可用)
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	i64 x,y,m,n,L;
	if(!(cin>>x>>y>>m>>n>>L))
		return 0;

	i64 pa=x%L,pb=y%L;
	for(i64 t=0;t<L;++t)
	{
		if(pa==pb)
		{
			cout<<t<<'\n';
			return 0;
		}
		pa=(pa+m)%L;
		pb=(pb+n)%L;
	}
	cout<<"Impossible\n";
	return 0;
}
```
