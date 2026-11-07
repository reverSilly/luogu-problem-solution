# P1829 [集训队互测 2010] Crash的数字表格 / JZPTAB

> 难度：NOI+ · 标签：莫比乌斯反演、整除分块、线性筛

## 1. 重新读题

### 1.1 问题重述（人话版）

给定 $n,m$，求

$$
\sum_{i=1}^{n}\sum_{j=1}^{m}\operatorname{lcm}(i,j)\pmod{20101009},
$$

其中 $\operatorname{lcm}(i,j)=\dfrac{ij}{\gcd(i,j)}$。

### 1.2 关键约束与规模暗示

- $1\le n,m\le 10^{7}$。这个规模排除了 $O(nm)$（最大 $10^{14}$）以及任何带 $\log$ 的逐对枚举；通常意味着「线性筛预处理 + 每次询问 $O(\sqrt n)$ 或 $O(n)$」。
- 模数 $20101009$ 很小（约 $2\times 10^7$），且不保证是质数（题目没有说明）。凡是需要「除以 $2$」的地方都必须在整数域里先除干净，**不能**用逆元。
- $10^7$ 的数组用 `int` 存需要约 $40\text{ MB}$，这是可以接受的；但不能再开好几个同样大的数组，需要尽量复用。
- 答案只需要一个数，说明应当把双重求和**合并**成一个单重求和，再用整除分块。

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：把 $\gcd$ 提出的标准分解

直接算 $\operatorname{lcm}$ 的最大障碍是分母上的 $\gcd$。令

$$
d=\gcd(i,j),\qquad i=d a,\quad j=d b,\quad \gcd(a,b)=1 .
$$

则 $\operatorname{lcm}(i,j)=\dfrac{ij}{d}=d\,a\,b$。于是

$$
A=\sum_{i\le n}\sum_{j\le m}\operatorname{lcm}(i,j)
 =\sum_{d=1}^{\min(n,m)} d\cdot F\!\left(\left\lfloor\frac nd\right\rfloor,\left\lfloor\frac md\right\rfloor\right),
$$

其中

$$
F(N,M)=\sum_{\substack{a\le N\\ b\le M\\ \gcd(a,b)=1}} ab .
$$

这一步是纯粹的换元：每个数对 $(i,j)$ 有唯一的 $d=\gcd(i,j)$，所以不重不漏。

### 2.2 化简：互质对上的加权和 —— 莫比乌斯反演

对互质条件使用标准恒等式

$$
[\gcd(a,b)=1]=\sum_{k\mid \gcd(a,b)}\mu(k).
$$

**引理 1（该恒等式）**：对任意正整数 $a,b$，$\sum_{k\mid\gcd(a,b)}\mu(k)=[\gcd(a,b)=1]$。

**证明**：设 $G=\gcd(a,b)$。若 $G=1$，求和只有 $k=1$，$\mu(1)=1$，等于 $1$。若 $G>1$，写 $G=\prod p_i^{e_i}$，$e_i\ge1$。因为 $\mu(k)\ne0$ 只在 $k$ 无平方因子时成立，所以只有 $k=\prod_{i\in S}p_i$（$S\subseteq\{1,\dots,r\}$，$r$ 为不同素因子个数）有贡献，且 $\mu(k)=(-1)^{|S|}$。于是总和 $=\sum_{S}(-1)^{|S|}=(1-1)^r=0$。$\blacksquare$

代入 $F$：

$$
F(N,M)=\sum_{a\le N}\sum_{b\le M}ab\sum_{k\mid a,\ k\mid b}\mu(k)
=\sum_{k\le\min(N,M)}\mu(k)k^{2}
\Bigl(\sum_{a'\le N/k}a'\Bigr)\Bigl(\sum_{b'\le M/k}b'\Bigr).
$$

记 $f(x)=\dfrac{x(x+1)}2$（$1+2+\cdots+x$），则

$$
\boxed{F(N,M)=\sum_{k\le\min(N,M)}\mu(k)\,k^{2}\,f\!\left(\left\lfloor\frac Nk\right\rfloor\right)f\!\left(\left\lfloor\frac Mk\right\rfloor\right)} .
$$

### 2.3 把两层和合并：关键的一步换序

现在 $A=\sum_d d\sum_k \mu(k)k^2 f(\lfloor n/(dk)\rfloor)f(\lfloor m/(dk)\rfloor)$。如果老老实实对每个 $d$ 单独算一遍 $F$，复杂度是

$$
\sum_{d\le\min(n,m)}\min\!\left(\frac nd,\frac md\right),
$$

$n=m=10^7$ 时约为 $1.6\times 10^8$，而且每一步还要访问 $\mu$ 数组，常数不小；更重要的是，这样得不到「一个」可以整除分块的形式。

**动机**：两个求和的下标只以乘积 $dk$ 的形式出现在 $f$ 里，所以应该把 $T=dk$ 当作新的求和变量。

令 $T=dk$，则

$$
A=\sum_{T=1}^{\min(n,m)} f\!\left(\left\lfloor\frac nT\right\rfloor\right)f\!\left(\left\lfloor\frac mT\right\rfloor\right)W(T),
\qquad
W(T)=\sum_{\substack{d\mid T\\ k=T/d}} d\cdot\mu(k)\,k^{2}.
$$

把 $k=T/d$ 代入：

$$
W(T)=\sum_{d\mid T} d\cdot\mu\!\left(\frac Td\right)\left(\frac Td\right)^2
= T\sum_{d\mid T}\frac Td\,\mu\!\left(\frac Td\right)
= T\sum_{e\mid T} e\,\mu(e)\qquad(e=T/d).
$$

**引理 2（$W$ 的显式公式）**：$\displaystyle W(T)=T\prod_{p\mid T}(1-p)$，其中乘积遍历 $T$ 的所有不同素因子。

**证明**：先看 $H(T)=\sum_{e\mid T}e\mu(e)$。函数 $e\mapsto e\mu(e)$ 是积性的（两个积性函数的乘积仍积性），而「积性函数的因子和」也是积性的：若 $\gcd(u,v)=1$，则 $T=uv$ 的每个因子唯一写成 $e_1e_2$（$e_1\mid u,e_2\mid v$），且 $e_1e_2\mu(e_1e_2)=e_1\mu(e_1)\cdot e_2\mu(e_2)$，求和后 $H(uv)=H(u)H(v)$。所以只需算素数幂 $T=p^a$：

$$
H(p^a)=\sum_{e\mid p^a}e\mu(e)=1+p\cdot(-1)+0+\cdots+0=1-p .
$$

于是 $H(T)=\prod_{p\mid T}(1-p)$，乘以 $T$ 即得。$\blacksquare$

检验几个小值（防止公式抄错）：$W(1)=1$；$W(2)=2(1-2)=-2$；$W(3)=-6$；$W(4)=4(1-2)=-4$；$W(6)=6(1-2)(1-3)=12$。用定义 $\sum_{d\mid T}d\mu(T/d)(T/d)^2$ 逐个验算 $T=4$：$d=1,2,4$ 分别对应 $e=4,2,1$，得 $1\cdot0+2\cdot(-1)\cdot4+4\cdot1\cdot1=-4$，与 $-4$ 一致。

> **失败尝试的极简反例**：如果猜 $W(T)=T\mu(T)$，则 $T=4$ 时得 $0$，而正确答案是 $-4$；如果猜 $W(T)=\varphi(T)T$，$T=2$ 时得 $2$，正确答案是 $-2$。所以必须保留 $\prod(1-p)$ 这个因子。

### 2.4 引理：线性筛递推 $W$

**引理 3**：$W(1)=1$；对素数 $p$，$W(p)=p(1-p)$；并且对任意 $i$ 与素数 $q$：

$$
W(iq)=\begin{cases}
q\,W(i), & q\mid i,\\
W(i)\,W(q), & q\nmid i.
\end{cases}
$$

**证明**：
- 第一式：若 $q\mid i$，则 $iq$ 与 $i$ 的不同素因子集合相同，于是由引理 2，
  $W(iq)=iq\prod_{p\mid i}(1-p)=q\cdot i\prod_{p\mid i}(1-p)=qW(i)$。
- 第二式：若 $q\nmid i$，则 $\gcd(i,q)=1$，而 $W(T)=T\prod_{p\mid T}(1-p)$ 显然是积性函数（两个积性函数 $T$ 与 $\prod_{p\mid T}(1-p)$ 的乘积），故 $W(iq)=W(i)W(q)$。$\blacksquare$

这两条正好是线性筛的标准递推形式，因此可以在 $O(\min(n,m))$ 时间内筛出所有 $W(T)$ 并同时做前缀和。

### 2.5 整除分块：证明区间只有 $O(\sqrt n)$ 段

**引理 4**：对固定 $n$，$\lfloor n/x\rfloor$ 在 $x\in[1,n]$ 上只有 $O(\sqrt n)$ 个不同取值。

**证明**：把 $x$ 分成 $x\le\sqrt n$ 与 $x>\sqrt n$ 两类。第一类只有 $\lfloor\sqrt n\rfloor$ 个 $x$，取值数不超过它；第二类中 $\lfloor n/x\rfloor<\sqrt n$，取值数不超过 $\sqrt n$。合计 $O(\sqrt n)$。$\blacksquare$

同时 $f(\lfloor n/T\rfloor)f(\lfloor m/T\rfloor)$ 在 $\lfloor n/T\rfloor$ 与 $\lfloor m/T\rfloor$ 都不变的区间上不变。从 $l$ 开始，满足 $\lfloor n/T\rfloor=\lfloor n/l\rfloor$ 的最大 $T$ 是 $\lfloor n/\lfloor n/l\rfloor\rfloor$，同理对 $m$，故一块的右端点为

$$
r=\min\left(\left\lfloor\frac{n}{\lfloor n/l\rfloor}\right\rfloor,\
\left\lfloor\frac{m}{\lfloor m/l\rfloor}\right\rfloor,\
\min(n,m)\right).
$$

在块内用前缀和 $\sum_{T=l}^{r}W(T)$ 乘上公共的 $f$ 值即可。由引理 4，块数为 $O(\sqrt n+\sqrt m)$。

### 2.6 为什么答案里的除法不会出问题

$f(x)=x(x+1)/2$。$x(x+1)$ 必为偶数，所以先在 64 位整数里算 $x(x+1)$ 再除以 $2$，最后取模，完全不需要模逆元。注意 $x$ 最大 $10^7$，$x(x+1)\le 10^{14}$，在 `long long` 内。

### 2.7 前缀和的符号问题

$W(T)\bmod p$ 可能为负（例如 $W(2)=-2$），筛的时候要 `(x%p+p)%p` 归一到 $[0,p)$。前缀和每一步都只有两个小于 $p$ 的数相加，不会超过 `int`（$2p\approx 4\times10^7$），但块内作差时仍要加 $p$ 再取模。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 令 $d=\gcd(i,j)$ 提出公因子，把 $\operatorname{lcm}$ 拆成 $d\cdot ab$，得到「互质对的加权和」。
- **classic problem solution** — 莫比乌斯反演消去互质条件，把 $F(N,M)$ 变成 $\sum_k\mu(k)k^2f(\lfloor N/k\rfloor)f(\lfloor M/k\rfloor)$。
- **reduce to easier problem** — 设 $T=dk$ 合并两层求和，把内层写成只依赖 $T$ 的系数 $W(T)$。
- **explore its property** — 证明 $W$ 是积性函数且 $W(p^a)=p^a(1-p)$，得到显式公式并设计线性筛递推。
- **classic problem solution** — 对 $\lfloor n/T\rfloor,\lfloor m/T\rfloor$ 做整除分块，用 $W$ 的前缀和一次算出答案。

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, m` | `int` | 表格行数、列数 | 读入 | $\le 10^7$ |
| `mn` | `int` | $\min(n,m)$ | 计算 | 筛的上界 |
| `g[T]` | `int [10000005]` | 正文的 $W(T)=T\prod_{p\mid T}(1-p)$，筛完后原地变成前缀和 $\sum_{t\le T}W(t)\bmod p$ | `g[1]=1` | 权值 / weight |
| `comp[i]` | `bool [10000005]` | 线性筛合数标记 | 全 `false` | composite |
| `primes[j]` | `int [700005]` | 筛出的素数表 | 空，`cnt=0` | 素数 |
| `p` | `constexpr int` | 模数 $20101009$ | 常量 | modulus |
| `ans` | `i64` | 最终答案 | $0$ | 输出它 |

### 4.2 算法步骤

1. 读入 $n,m$，令 `mn=min(n,m)`。
2. **线性筛 $W$**：`g[1]=1`；对 $i=2\dots mn$：
   - 若 $i$ 未被标记，则 $i$ 是素数，`g[i]=i*(1-i) mod p`，加入素数表；
   - 枚举素数 $q$，若 $i q>mn$ 则退出；标记 $i q$ 合数；若 $q\mid i$，`g[i*q]=g[i]*q`，并 `break`；否则 `g[i*q]=g[i]*g[q]`。
3. **原地前缀和**：对 $i=2\dots mn$，`g[i]=(g[i]+g[i-1])%p`。
4. **整除分块**：`l=1`；当 `l<=mn`：
   - `qn=n/l`，`qm=m/l`，`r=min(n/qn, m/qm, mn)`；
   - 区间和 `pre=g[r]-g[l-1]`（加 $p$ 取模）；
   - `ans += pre * f(qn) * f(qm)`，其中 `f(x)=x*(x+1)/2 mod p`；
   - `l=r+1`。
5. 输出 `ans`。

### 4.3 复杂度

- 时间：线性筛 $O(\min(n,m))$；整除分块 $O(\sqrt n+\sqrt m)$（约 $1.3\times10^4$ 次块）。总时间 $O(\min(n,m))$。
- 空间：`g` 与 `comp` 各约 $10^7$ 个元素，加素数表，合计约 $54\text{ MB}$。

## 5. 示例代码

```cpp
//
// 题目：P1829 [集训队互测 2010] Crash的数字表格 / JZPTAB
// https://www.luogu.com.cn/problem/P1829
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{10000005};
constexpr int p{20101009};

int n,m,mn;
int g[mxn];
bool comp[mxn];
int primes[700005];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	mn=min(n,m);
	g[1]=1;
	int cnt{0};
	for(int i=2;i<=mn;++i)
	{
		if(!comp[i])
		{
			primes[cnt++]=i;
			g[i]=(int)((1LL*i*(1-i))%p);
			if(g[i]<0)
				g[i]+=p;
		}
		for(int j=0;j<cnt;++j)
		{
			int q=primes[j];
			if(1LL*i*q>mn)
				break;
			comp[i*q]=true;
			if(i%q==0)
			{
				g[i*q]=(int)(1LL*g[i]*q%p);
				break;
			}
			g[i*q]=(int)(1LL*g[i]*g[q]%p);
		}
	}
	for(int i=2;i<=mn;++i)
	{
		g[i]+=g[i-1];
		if(g[i]>=p)
			g[i]-=p;
	}
	i64 ans{0};
	for(int l=1,r;l<=mn;l=r+1)
	{
		int qn=n/l,qm=m/l;
		r=min({n/qn,m/qm,mn});
		i64 pre=g[r]-(l>1?g[l-1]:0);
		if(pre<0)
			pre+=p;
		i64 fn=1LL*qn*(qn+1)/2%p;
		i64 fm=1LL*qm*(qm+1)/2%p;
		ans=(ans+pre*fn%p*fm)%p;
	}
	cout<<ans<<'\n';
	return 0;
}
```
