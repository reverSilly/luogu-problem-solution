# P1891 疯狂 LCM

## 1. 重新读题

### 1.1 问题重述（人话版）

有 $T$ 组询问，每组给出一个正整数 $n$，求

$$
F(n)=\sum_{i=1}^{n}\operatorname{lcm}(i,n).
$$

### 1.2 关键约束与规模暗示

- $T\le 3\times 10^5$，$n\le 10^6$。
- 若对每组询问花 $O(n\log n)$ 逐个求 $\gcd$，最坏 $3\times 10^5\times 10^6=3\times 10^{11}$，必然超时。因此必须把每组询问降到 $O(1)$ 或 $O(\log n)$。
- $n\le 10^6$ 又暗示可以接受 $O(M\log M)$ 的**预处理**（$M=10^6$），其中 $M\log M\approx 1.4\times 10^7$。
- 答案量级：$\operatorname{lcm}(i,n)\le in\le n^2$，$n$ 项之和约 $n^3$ 量级？先别急，下面会证明真实上界是 $O(n^3)$ 但常数很小；对 $n=10^6$ 需要 `long long` 存答案，并要仔细估计它到底有多大。
- 题目带「数学、欧拉函数」标签，提示应当用 $\gcd$ 的分类或 $\varphi$ 的和式。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：把 $\gcd$ 提出来

直接对每个 $i$ 求 $\gcd(i,n)$ 太慢。先写出

$$
\operatorname{lcm}(i,n)=\frac{i\cdot n}{\gcd(i,n)}.
$$

分母只可能是 $n$ 的约数，这提示**按 $g=\gcd(i,n)$ 分类**。对固定的 $g\mid n$，满足 $\gcd(i,n)=g$ 的 $i$ 可以写成 $i=gk$，其中

$$
1\le k\le \frac{n}{g}=d,\qquad \gcd(k,d)=1.
$$

此时

$$
\operatorname{lcm}(i,n)=\frac{gk\cdot n}{g}=kn.
$$

这是一个非常干净的结果：**把 $i$ 按 $\gcd$ 分组后，$n$ 被提到了外面，剩下的只是「与 $d$ 互素的 $k$ 之和」**。

### 2.2 引理 1（按 $\gcd$ 分类的求和式）

**引理 1.**

$$
F(n)=n\sum_{d\mid n}S(d),\qquad S(d)=\sum_{\substack{1\le k\le d\\ \gcd(k,d)=1}}k.
$$

**证明.** 每个 $i\in[1,n]$ 有唯一的 $g=\gcd(i,n)$，且 $g\mid n$。因此

$$
\sum_{i=1}^n\operatorname{lcm}(i,n)
=\sum_{g\mid n}\ \sum_{\substack{1\le i\le n\\ \gcd(i,n)=g}}\frac{in}{g}.
$$

对固定的 $g$，令 $d=n/g$、$i=gk$。条件 $\gcd(i,n)=g$ 等价于 $\gcd(gk,gd)=g$，即 $\gcd(k,d)=1$；又 $1\le i\le n$ 等价于 $1\le k\le d$。此时被加项为 $kn$。于是

$$
F(n)=\sum_{g\mid n}\ \sum_{\substack{1\le k\le d\\ \gcd(k,d)=1}}kn
=n\sum_{d\mid n}\ \sum_{\substack{1\le k\le d\\ \gcd(k,d)=1}}k
=n\sum_{d\mid n}S(d).
$$

注意 $d=n/g$ 也取遍 $n$ 的全部约数，所以第二个等号成立。$\blacksquare$

### 2.3 引理 2（互素 $k$ 的和）

**引理 2.**

$$
S(1)=1,\qquad S(d)=\frac{d\,\varphi(d)}{2}\quad(d\ge 2).
$$

**证明.**

- $d=1$：只有 $k=1$，故 $S(1)=1$。
- $d=2$：与 $2$ 互素的 $k$ 只有 $1$，$S(2)=1=2\cdot\varphi(2)/2$。
- $d>2$：考虑映射 $k\mapsto d-k$。若 $\gcd(k,d)=1$，则 $\gcd(d-k,d)=\gcd(k,d)=1$，所以该映射在「与 $d$ 互素的剩余类」上封闭，并且是自身的逆，从而是双射。
  - 它没有不动点：$k=d-k$ 推出 $k=d/2$，但 $d>2$ 时 $\gcd(d/2,d)=d/2\ge 2$，与 $\gcd(k,d)=1$ 矛盾。
  - 因此所有元素可以两两配成 $(k,d-k)$，每对之和为 $d$。
  - 互素元素共有 $\varphi(d)$ 个，配成 $\varphi(d)/2$ 对，故 $S(d)=d\varphi(d)/2$。

$\blacksquare$

### 2.4 合并：一个非常漂亮的闭式

把引理 2 代入引理 1：

$$
F(n)=n\left(1+\frac12\sum_{\substack{d\mid n\\ d\ge 2}}d\varphi(d)\right).
$$

记

$$
H(n)=\sum_{d\mid n}d\varphi(d).
$$

因为 $H(n)$ 中 $d=1$ 的项恰为 $1\cdot\varphi(1)=1$，所以

$$
1+\frac12\sum_{\substack{d\mid n\\ d\ge2}}d\varphi(d)
=1+\frac{H(n)-1}{2}
=\frac{H(n)+1}{2}.
$$

于是得到最终公式

$$
\boxed{F(n)=\frac{n\bigl(H(n)+1\bigr)}{2}},\qquad H(n)=\sum_{d\mid n}d\varphi(d).
$$

### 2.5 引理 3（答案一定是整数，并顺便给出上界）

**引理 3.**

1. 对任意 $d\ge 2$，$d\varphi(d)$ 是偶数；
2. 因此 $H(n)$ 是奇数，$H(n)+1$ 是偶数，$\dfrac{n(H(n)+1)}{2}$ 是整数。

**证明.**

1. 若 $d$ 为偶数，则 $d\varphi(d)$ 含有因子 $2$，是偶数。若 $d$ 为奇且 $d\ge 3$，则用引理 2 中 $d>2$ 的配对论证：$k\mapsto d-k$ 是互素剩余类上的无不动点双射，所以元素个数 $\varphi(d)$ 是偶数，乘积也为偶数。$d=2$ 时 $d\varphi(d)=2$ 也是偶数。
2. $H(n)=1\cdot\varphi(1)+\sum_{d\mid n,d\ge2}d\varphi(d)=1+\text{偶数}$，故为奇数。

$\blacksquare$

**上界.** 先用一个经典恒等式：$\sum_{d\mid n}\varphi(d)=n$。证明：把 $1,\dots,n$ 按 $\gcd(i,n)=g$ 分类，满足 $\gcd(i,n)=g$ 的 $i$ 的个数是 $\varphi(n/g)$，因此 $n=\sum_{g\mid n}\varphi(n/g)=\sum_{d\mid n}\varphi(d)$（做替换 $d=n/g$）。

于是

$$
H(n)=\sum_{d\mid n}d\varphi(d)\le n\sum_{d\mid n}\varphi(d)=n^2,
$$

$$
F(n)=\frac{n(H(n)+1)}{2}\le\frac{n(n^2+1)}{2}\approx 5\times 10^{17}\quad(n\le 10^6),
$$

稳稳落在 `long long`（约 $9.2\times 10^{18}$）范围内。

### 2.6 怎样快速得到所有 $H(n)$

$H$ 是「$d\varphi(d)$ 加到它的所有倍数上」得到的：

$$
H(m)=\sum_{d\mid m}d\varphi(d).
$$

因此流程是：

1. 线性筛求出 $1\dots M$ 的 $\varphi$；
2. 枚举 $d=1\dots M$，把 $d\varphi(d)$ 加到 $H[d],H[2d],H[3d],\dots$ 上。

总代价 $\sum_{d=1}^{M}M/d=O(M\log M)\approx 1.4\times 10^7$，可以接受。之后每组询问 $O(1)$。

### 2.7 失败尝试记录

1. **对每个 $i$ 求 $\gcd$ 再累加**：单组 $O(n\log n)$，$T$ 组爆炸。
2. **每组单独筛 $\varphi$**：更慢，且完全没必要，因为上界 $10^6$ 固定，一次预处理即可。
3. **忘记 $d=1$ 的特判**：$d\varphi(d)/2=1/2$ 不是整数，会得到错误的小数结果；必须把 $1$ 单独拿出来。
4. **用 `int` 存答案**：$H(n)$ 可达 $10^{12}$，答案可达 $5\times 10^{17}$，必须 `long long`。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 把 $\operatorname{lcm}$ 写成 $in/\gcd$，按 $\gcd$ 分类。
- **reduce to easier problem** — 每类只剩「与 $d$ 互素的数之和」。
- **prove a number-theoretic lemma** — 用 $k\mapsto d-k$ 配对得到 $S(d)=d\varphi(d)/2$，并单独处理 $d=1,2$。
- **classic problem solution** — 线性筛出 $\varphi$，再对倍数做调和级数累加，预处理出 $H$。
- **answer queries in O(1)** — 每组直接套闭式，注意 `long long`。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `mxv` | `constexpr int` | 预处理上界 $10^6+5$ | 常量 | max value |
| `phi[x]` | `int [1000005]` | 欧拉函数 $\varphi(x)$ | 线性筛，`phi[1]=1` | phi |
| `pr[]` | `int [1000005]` | 线性筛素数表 | 空 | primes |
| `comp[x]` | `bool [1000005]` | 合数标记 | `false` | composite |
| `acc[m]` | `i64 [1000005]` | 即 $H(m)=\sum_{d\mid m}d\varphi(d)$ | 全 $0$ | accumulate |
| `n` | `int` | 当前询问 | 读入 | |
| `t` | `int` | 询问组数 | 读入 | |

### 4.2 算法步骤

1. 线性筛：`phi[1]=1`；从 $2$ 到 $M$ 枚举 $i$，若未标记为合数则 $i$ 为素数，`phi[i]=i-1`；用素数表更新 $i\cdot p$，按 $p\mid i$ 与否分别乘 $p$ 或 $p-1$。
2. 对每个 $d=1\dots M$，计算 $v=d\cdot\varphi(d)$，并枚举 `m=d; m<=M; m+=d`，执行 `acc[m]+=v`。
3. 读入 $T$；每组读入 $n$，输出 `(i64)n*(1+acc[n])/2`。

### 4.3 复杂度分析

- **预处理**：线性筛 $O(M)$；调和级数累加 $O(M\log M)$。
- **单组询问**：$O(1)$。
- **总时间**：$O(M\log M+T)$，$M=10^6$。
- **空间**：$O(M)$，`phi/pr/comp/acc` 合计约 $17$ MB。

---

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxv{1000005};

int phi[mxv],pr[mxv],cntp;
bool comp[mxv];
i64 acc[mxv];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	phi[1]=1;
	for(int i=2;i<mxv;++i)
	{
		if(!comp[i])
		{
			pr[cntp++]=i;
			phi[i]=i-1;
		}
		for(int j=0;j<cntp&&(i64)pr[j]*i<mxv;++j)
		{
			comp[pr[j]*i]=true;
			if(i%pr[j]==0)
			{
				phi[i*pr[j]]=phi[i]*pr[j];
				break;
			}
			phi[i*pr[j]]=phi[i]*(pr[j]-1);
		}
	}
	for(int d=1;d<mxv;++d)
	{
		i64 v=(i64)d*phi[d];
		for(int m=d;m<mxv;m+=d)
			acc[m]+=v;
	}
	int t;
	cin>>t;
	while(t--)
	{
		int n;
		cin>>n;
		cout<<(i64)n*(1+acc[n])/2<<'\n';
	}
	return 0;
}
```
