# P1951 [Aboi 2077] SL2(Z/NZ)

## 1. 重新读题

### 1.1 问题重述（人话版）

给定模数 $N$，统计所有满足
$$
ad-bc\equiv 1\pmod N
$$
的四元组 $(a,b,c,d)$ 的个数，其中 $a,b,c,d$ 都是 $0\le\cdot<N$ 的整数。换句话说，要求模 $N$ 意义下行列式为 $1$ 的 $2\times2$ 矩阵数量，也就是 $\bigl|\mathrm{SL}_2(\mathbb Z/N\mathbb Z)\bigr|$。

有多组询问：第一行 $T$，之后每行一个 $N$，对每组输出答案对 $998244353$ 取模的结果。

### 1.2 关键约束与规模暗示

- $1\le T\le 10$，$1\le N\le 10^{18}$。
- 子任务按 $N$ 分层：$\le 50$（$10$ 分）、$\le 200$、$\le 10^3$、$\le 10^6$、$\le 10^9$、$\le 10^{18}$。
- 前几档可以用 $O(N^4)$、$O(N^3)$ 之类的直接枚举；$\le 10^6$ 可以筛出质数；$\le 10^9$ 可以 $\sqrt N$ 试除分解；而 $\le 10^{18}$ 说明**必须能对 $10^{18}$ 级别的数做质因数分解**，这正是 Pollard-Rho 的用武之地。
- 分层如此规整（每加一档数量级就多一种算法），强烈暗示本题存在一个**只依赖于 $N$ 的质因数**的封闭公式，然后问题退化为「分解 $N$」。

---

## 2. 思维链（Chain-of-thought）

### 2.1 先手算几个小 $N$，找规律

$N=1$：模 $1$ 意义下只有一个数 $0$，所有 $4$ 个位置都只能取 $0$，行列式 $0\equiv 1\pmod 1$ 成立，故答案为 $1$。

$N=2$：$\mathbb F_2$ 上行列式非零的矩阵数，等于「第一列非零」$3$ 种选法、第二列与第一列不共线，剩 $4-2=2$ 种，共 $6$ 个。直接枚举 $16$ 个矩阵也能验证是 $6$。

$N=3$：第一列非零 $9-1=8$ 种，第二列不在第一列张成的直线上，剩 $9-3=6$ 种，共 $48$ 个。

$N=4$：暴力数一下是 $48$。

$N=6$：暴力是 $144$。

把这些数写开：

| $N$ | 答案 | 猜测的写法 |
| :---: | :---: | :--- |
| $1$ | $1$ | $1^3$ |
| $2$ | $6$ | $2^3\cdot\frac34$ |
| $3$ | $24$ | $3^3\cdot\frac89$ |
| $4$ | $48$ | $4^3\cdot\frac34$ |
| $6$ | $144$ | $6^3\cdot\frac34\cdot\frac89$ |

立刻看出
$$
\boxed{\ \bigl|\mathrm{SL}_2(\mathbb Z/N\mathbb Z)\bigr|=N^3\prod_{p\mid N}\left(1-\frac1{p^2}\right)\ }
$$
其中乘积遍历 $N$ 的所有**不同**质因子。$N=4$ 只乘一次 $\frac34$（因为 $2$ 只算一个质因子），这与 $4^3\cdot\frac34=48$ 吻合。

下面不靠「猜」，把这个公式拆成两块严格证明：**质数幂**与**中国剩余定理（乘法性）**。

### 2.2 先解决简单版：$N=p^k$ 是质数幂

先去数 $(a,b)$ 这一行。若 $p\mid a$ 且 $p\mid b$，那么对任意 $c,d$ 都有
$$
ad-bc\equiv 0\pmod p,
$$
不可能 $\equiv 1\pmod{p^k}$，这样的行贡献 $0$。

否则 $(a,b)$ 不同时被 $p$ 整除，即 $p\nmid a$ 或 $p\nmid b$，于是 $\gcd(a,b,p^k)=1$。

**引理 1（一行合法时，后续行恰有 $p^k$ 种）**
若 $\gcd(a,b,p^k)=1$，则方程
$$
ad-bc\equiv 1\pmod{p^k}
$$
关于 $(c,d)\in(\mathbb Z/p^k\mathbb Z)^2$ 恰有 $p^k$ 组解。

**证明**：考虑映射
$$
L:\ (\mathbb Z/p^k\mathbb Z)^2\to\mathbb Z/p^k\mathbb Z,\qquad L(c,d)=ad-bc .
$$
它是加法群同态。

先证 $L$ 满射。若 $p\nmid a$，则 $a$ 在模 $p^k$ 下可逆，取 $c=0$ 得 $d\equiv a^{-1}\pmod{p^k}$，于是对任意目标值都能取到；若 $p\mid a$，由 $\gcd(a,b,p^k)=1$ 得 $p\nmid b$，同理取 $d=0$ 得 $c\equiv -b^{-1}\pmod{p^k}$。两种情形都说明 $L$ 的值域是整个 $\mathbb Z/p^k\mathbb Z$。

同态的纤维是核的陪集，因而所有纤维大小相同。定义域有 $(p^k)^2=p^{2k}$ 个元素，值域有 $p^k$ 个元素，满射迫使每条纤维大小为
$$
\frac{p^{2k}}{p^k}=p^k.
$$
这就是方程的解数。$\square$

于是
$$
\#(p^k)=p^k\cdot\bigl(\#\{(a,b):\ p\nmid a\text{ 或 }p\nmid b\}\bigr).
$$
而「$p\mid a$ 且 $p\mid b$」的 $(a,b)$ 有 $p^{k-1}\cdot p^{k-1}=p^{2k-2}$ 个，所以
$$
\#(p^k)=p^k\bigl(p^{2k}-p^{2k-2}\bigr)=p^{3k}-p^{3k-2}=p^{3k}\left(1-\frac1{p^2}\right).
$$

**引理 2（质数幂公式）** 对任意质数 $p$ 与 $k\ge 1$，
$$
\bigl|\mathrm{SL}_2(\mathbb Z/p^k\mathbb Z)\bigr|=p^{3k}\left(1-\frac1{p^2}\right).
$$

**证明**：即上面的计数：先按第一行分两类，坏行贡献 $0$，好行每行恰贡献 $p^k$ 种后续行，好行共有 $p^{2k}-p^{2k-2}$ 个，乘起来即得。$\square$

（小检查：$k=1$ 时得到 $p^3-p=p(p^2-1)$，与 $\mathbb F_p$ 上「第一列非零、第二列不共线」的几何计数一致。）

### 2.3 再把各质数幂拼起来：为什么是乘积

**引理 3（对互素模数乘法性）**
若 $\gcd(M,N)=1$，则
$$
\bigl|\mathrm{SL}_2(\mathbb Z/MN\mathbb Z)\bigr|
=\bigl|\mathrm{SL}_2(\mathbb Z/M\mathbb Z)\bigr|\cdot
\bigl|\mathrm{SL}_2(\mathbb Z/N\mathbb Z)\bigr|.
$$

**证明**：中国剩余定理给出环同构
$$
\varphi:\mathbb Z/MN\mathbb Z\to\mathbb Z/M\mathbb Z\times\mathbb Z/N\mathbb Z,\qquad
\varphi(x)=(x\bmod M,\ x\bmod N).
$$
验证三件事：

1. **良定义且是同态**：$x+y$ 与 $xy$ 的分量分别等于分量相加、分量相乘，直接由模运算性质得到。
2. **单射**：若 $\varphi(x)=0$，则 $M\mid x$ 且 $N\mid x$，因 $\gcd(M,N)=1$ 得 $MN\mid x$，即 $x=0$。
3. **满射**：两边元素个数都是 $MN$，单射从有限集到同大小有限集必为双射。

所以 $\varphi$ 是环同构。

把它**逐分量**作用在矩阵上：
$$
\Phi:\ M_2(\mathbb Z/MN\mathbb Z)\to M_2(\mathbb Z/M\mathbb Z)\times M_2(\mathbb Z/N\mathbb Z),
\qquad \Phi(A)=(\varphi(A_{ij}))_{ij}.
$$
矩阵的加法、乘法都是对每个位置独立进行的，因此 $\Phi$ 是矩阵环的同构，特别地是**双射**。

行列式 $\det(A)=A_{11}A_{22}-A_{12}A_{21}$ 是四个元素的多项式，系数为整数。多项式运算与环同态交换，故
$$
\det(\Phi(A))=\bigl(\det(A)\bmod M,\ \det(A)\bmod N\bigr).
$$
因此 $\det(A)\equiv 1\pmod{MN}$ 当且仅当两个分量都满足 $\det\equiv 1$（因为 $M\mid(\det-1)$ 与 $N\mid(\det-1)$ 同时成立等价于 $MN\mid(\det-1)$，这里再次用了 $\gcd(M,N)=1$）。于是 $\Phi$ 把 $\mathrm{SL}_2(\mathbb Z/MN\mathbb Z)$ 双射到两个 $\mathrm{SL}_2$ 的笛卡尔积上，两边元素数相乘。$\square$

**定理（主公式）** 设 $N=\prod_{i=1}^{s}p_i^{k_i}$ 是 $N$ 的质因数分解（$p_i$ 两两不同），则
$$
\bigl|\mathrm{SL}_2(\mathbb Z/N\mathbb Z)\bigr|
=\prod_{i=1}^{s}p_i^{3k_i}\left(1-\frac1{p_i^2}\right)
=N^3\prod_{p\mid N}\left(1-\frac1{p^2}\right).
$$

**证明**：对 $s$ 归纳。$s=0$（即 $N=1$）时 $\mathbb Z/1\mathbb Z$ 只有一个元素，唯一的矩阵行列式为 $0\equiv 1\pmod 1$，答案 $1$，空乘积也等于 $1$。$s=1$ 是引理 2。归纳步把 $N$ 写成 $p_s^{k_s}\cdot(N/p_s^{k_s})$，两部分互素，用引理 3 拆开，前一部分用引理 2，后一部分用归纳假设，再注意 $\prod p_i^{3k_i}=N^3$。$\square$

### 2.4 落地到 $N\le10^{18}$：分解质因数

公式本身是 $O(1)$（在已分解的前提下），真正的工作量是**把 $N$ 分解**。$N$ 可达 $10^{18}$，$\sqrt N$ 试除是 $10^9$，$T=10$ 时不可能。

用 Pollard-Rho 分解：

- **Miller-Rabin** 判素。对 $n<2^{64}$，取前 $12$ 个质数 $2,3,\dots,37$ 作底数即可**确定性**判素（这是已知结论：前若干个质数底数足以覆盖 $2^{64}$ 范围；本题 $n\le10^{18}<2^{64}$，安全）。
- **Pollard-Rho** 找非平凡因子。随机 $c$，令 $f(x)=x^2+c\bmod n$，用 Floyd 判圈求 $\gcd(|x-y|,n)$；若得到 $n$ 就换一个 $c$ 重来。期望 $O(n^{1/4})$ 次迭代可以找到因子。
- 递归分解直到全部是质数，排序后去重。

乘法全部用 `__int128`（$10^{18}\times10^{18}$ 会爆 `long long`）。

### 2.5 模 $998244353$ 的细节，以及一个必须处理的坑

设 $\mathrm{MOD}=998244353$。我们要算
$$
(N\bmod \mathrm{MOD})^3\cdot\prod_{p\mid N}\frac{p^2-1}{p^2}\pmod{\mathrm{MOD}}.
$$
每一项用 $p^2$ 的逆元即可。但**若某个质因子 $p=\mathrm{MOD}$，逆元不存在**。

这时答案其实恒为 $0$，理由用 $p$-进赋值：把答案看成有理数
$$
A=N^3\prod_{q\mid N}\frac{q^2-1}{q^2}.
$$
虽然它是整数，但逐项看，$v_p(A)$ 满足
$$
v_p(A)\ge v_p(N^3)+v_p(p^2-1)-v_p(p^2)=3k-0-2=3k-2\ge 1
$$
（$k\ge1$ 是 $p$ 在 $N$ 中的指数；对 $q\ne p$ 的因子，$v_p\bigl((q^2-1)/q^2\bigr)\ge0$）。所以 $\mathrm{MOD}\mid A$，输出 $0$。

因此实现里：若某个质因子 $p\equiv0\pmod{\mathrm{MOD}}$，直接令答案为 $0$；否则正常乘逆元。

### 2.6 顺带验证

- $N=1$：空分解，答案 $1^3=1$，与定义一致（模 $1$ 下 $0\equiv1$）。
- $N=4$：$4^3\cdot\frac34=48$；$N=6$：$6^3\cdot\frac34\cdot\frac89=144$。
- $N=\mathrm{MOD}$：答案 $N(N^2-1)\equiv 0\pmod N$，程序输出 $0$，符合 2.5 的结论。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 手算 $N=1,2,3,4,6$，猜出 $N^3\prod_{p\mid N}(1-p^{-2})$。
- **consider easier version** — 先只做质数幂 $N=p^k$：按第一行分类，退化为一次线性同余方程计数。
- **classic problem solution** — 线性映射满射 + 纤维等大，得到每个好首行恰 $p^k$ 个解。
- **classic problem solution** — 中国剩余定理给出环同构，行列式是整系数多项式，所以计数乘法。
- **classic problem solution** — $10^{18}$ 分解：Miller-Rabin 判素 + Pollard-Rho 找因子。
- **handle the modular edge case** — 质因子恰为模数时逆元不存在，用 $p$-进赋值证明答案为 $0$。

---

## 4. 真实解法（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `T` | `int` | 询问组数 | 读入 | $\le10$ |
| `N` | `i64` | 当前模数 | 读入 | $\le10^{18}$ |
| `fac` | `vector<i64>` | $N$ 的全部质因子（含重数） | 清空 | factor |
| `mod` | `constexpr int` | $998244353$ | — | 输出模数 |
| `mulmod(a,b,c)` | 函数 | `__int128` 防溢出的模乘 | — | 中间量 $10^{36}$ |
| `qpow(a,b,c)` | 函数 | 模快速幂 | — | 逆元 / 判素 |
| `isp(n)` | 函数 | Miller-Rabin 确定性判素 | — | is prime |
| `rho(n)` | 函数 | 返回 $n$ 的一个非平凡因子 | — | Pollard-Rho |
| `factor(n)` | 函数 | 递归分解，结果压入 `fac` | — | 分治 |
| `ans` | `i64` | 当前询问答案 | `qpow(N%mod,3,mod)` | 先放 $N^3$ |

### 4.2 算法步骤

1. 读入 $T$，循环处理每组 $N$。
2. 若 $N=1$，`factor(1)` 直接返回，`fac` 为空。
3. 否则递归分解：`isp(n)` 为真则压入；否则 `rho(n)` 得到因子 $d$，递归分解 $d$ 与 $n/d$。
4. 对 `fac` 排序，按**不同质因子** $p$ 分段去重。
5. 初始化 `ans=(N mod mod)^3 mod mod`。
6. 对每个不同质因子 $p$：
   - 若 `p%mod==0`，令 `ans=0` 并跳出（2.5 的边界）；
   - 否则 `ans = ans·(p^2-1)·(p^2)^{-1} mod mod`。
7. 输出 `ans`。

### 4.3 复杂度分析

- 单次分解的期望复杂度为 $O\bigl(N^{1/4}\operatorname{polylog}N\bigr)$（Pollard-Rho 的期望迭代次数），Miller-Rabin 每次 $O(\log N)$ 次模乘，每个因子至多 $O(\log N)$ 轮。
- 模运算与去重 $O(\omega(N)\log\omega(N))$。
- 空间 $O(\omega(N))$，$\omega(N)\le 15$（$2^{60}>10^{18}$）。
- 对 $N\le10^{18}$、$T\le10$，实测毫秒级。

---

## 5. 示例代码

```cpp
//
// 题目：P1951 [Aboi 2077] SL2(Z/NZ)
// https://www.luogu.com.cn/problem/P1951
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
using i128=__int128;

constexpr int mod{998244353};

i64 mulmod(i64 a,i64 b,i64 c)
{
	return (i64)((i128)a*b%c);
}

i64 qpow(i64 a,i64 b,i64 c)
{
	i64 r=1%c;
	a%=c;
	for(;b;b>>=1,a=mulmod(a,a,c))
		if(b&1)
			r=mulmod(r,a,c);
	return r;
}

bool isp(i64 n)
{
	if(n<2)
		return false;
	static const int base[]{2,3,5,7,11,13,17,19,23,29,31,37};
	for(int p:base)
	{
		if(n==p)
			return true;
		if(n%p==0)
			return false;
	}
	i64 d=n-1;
	int s{0};
	while(!(d&1))
	{
		d>>=1;
		++s;
	}
	for(int p:base)
	{
		i64 x=qpow(p,d,n);
		if(x==1||x==n-1)
			continue;
		bool ok{false};
		for(int i=1;i<s;++i)
		{
			x=mulmod(x,x,n);
			if(x==n-1)
			{
				ok=true;
				break;
			}
		}
		if(!ok)
			return false;
	}
	return true;
}

mt19937_64 rn(1145141919810ull);

i64 rho(i64 n)
{
	if(n%2==0)
		return 2;
	while(true)
	{
		i64 c=rn()%(n-1)+1;
		i64 x=rn()%(n-1)+1,y=x;
		i64 d{1};
		auto f=[&](i64 v){return (i64)(((i128)v*v+c)%n);};
		while(d==1)
		{
			x=f(x);
			y=f(f(y));
			d=__gcd(x>y?x-y:y-x,n);
		}
		if(d!=n)
			return d;
	}
}

vector<i64>fac;

void factor(i64 n)
{
	if(n==1)
		return;
	if(isp(n))
	{
		fac.push_back(n);
		return;
	}
	i64 d=rho(n);
	factor(d);
	factor(n/d);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int T;
	cin>>T;
	while(T--)
	{
		i64 N;
		cin>>N;
		fac.clear();
		factor(N);
		sort(fac.begin(),fac.end());
		i64 ans=qpow(N%mod,3,mod);
		for(size_t i=0;i<fac.size();)
		{
			i64 p=fac[i];
			size_t j=i;
			while(j<fac.size()&&fac[j]==p)
				++j;
			i=j;
			if(p%mod==0)
			{
				ans=0;
				break;
			}
			i64 pm=p%mod;
			i64 num=mulmod((pm-1+mod)%mod,(pm+1)%mod,mod);
			i64 den=mulmod(pm,pm,mod);
			ans=mulmod(ans,mulmod(num,qpow(den,mod-2,mod),mod),mod);
		}
		cout<<ans<<'\n';
	}
	return 0;
}
```
