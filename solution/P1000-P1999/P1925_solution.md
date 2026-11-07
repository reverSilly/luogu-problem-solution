# P1925 最大划分乘积

## 1. 重新读题

### 1.1 人话重述

把正整数 $n$ 分成 $k$ 个相等的部分，每份为 $r=n/k$（$k$ 是正整数，$r$ 不必是整数），令
$$
p=r^k=\left(\frac nk\right)^k
$$
（共 $k$ 个 $r$ 相乘）。定义
$$
M(n)=\max_{k\in\mathbb Z_{>0}}\left(\frac nk\right)^k .
$$
若 $M(n)$ 是有限小数，则 $D(n)=-n$；若 $M(n)$ 是无限小数，则 $D(n)=n$。求
$$
D(5)+D(6)+\cdots+D(a),\qquad 5\le a\le 32767 .
$$

### 1.2 关键约束与规模暗示

- $a\le 32767$，可以逐个数处理，单个 $n$ 只要 $O(\log n)$ 或 $O(1)$ 即可。
- 核心有两件事：**怎样求使 $(n/k)^k$ 最大的整数 $k$**，以及**怎样判断这个有理数是不是有限小数**。
- 题面给的例子：$n=8$ 时 $M(n)=512/27$（无限），$n=11$ 时 $M(n)=14641/256$（有限），可以用来检验。

---

## 2. 思维链（Chain-of-thought）

### 2.1 连续化：最优点在哪里

对固定的 $n$，考虑实变量 $k>0$：
$$
g(k)=\ln\left[\left(\frac nk\right)^k\right]=k\ln\frac nk=k(\ln n-\ln k).
$$
求导：
$$
g'(k)=\ln\frac nk-1,\qquad g''(k)=-\frac1k<0 .
$$
因此 $g$ 是严格凹函数，$g'$ 严格递减。令 $g'(k)=0$：
$$
\ln\frac nk=1\iff \frac nk=e\iff k=\frac ne .
$$
于是：

**引理 1（连续最优点）**：$g$ 在 $(0,n/e)$ 上严格递增、在 $(n/e,+\infty)$ 上严格递减，唯一最大值点是 $k^*=n/e$。

**证明**：$g''(k)=-1/k<0$，所以 $g'$ 严格递减；$g'(k^*)=0$。当 $k<k^*$ 时 $\ln(n/k)>1$，$g'(k)>0$；当 $k>k^*$ 时 $g'(k)<0$。故 $g$ 先增后减，唯一极大值点即最大值点。$\blacksquare$

因为 $\ln$ 严格递增，最大化 $g(k)$ 与最大化 $(n/k)^k$ 等价。

**引理 2（整数最优点）**：设 $k^*=n/e$，则使 $g(k)$ 在整数 $k$ 上最大的 $k$ 必为 $\lfloor k^*\rfloor$ 或 $\lceil k^*\rceil$。

**证明**：由引理 1，$g$ 在整数点上的最大值只能出现在 $k^*$ 左右两侧最近的整数处：若整数 $k<\lfloor k^*\rfloor$，则 $k+1\le\lfloor k^*\rfloor<k^*$，$g$ 在 $[k,\lfloor k^*\rfloor]$ 上递增，故 $g(k)\le g(\lfloor k^*\rfloor)$；同理若整数 $k>\lceil k^*\rceil$，则 $g(k)\le g(\lceil k^*\rceil)$。所以最大值只需比较这两个点。$\blacksquare$

于是对每个 $n$，只需计算
$$
k_0=\left\lfloor\frac ne\right\rfloor,\qquad k_1=k_0+1,
$$
比较 $k_0\ln(n/k_0)$ 与 $k_1\ln(n/k_1)$，取较大者作为最优 $k$（若 $k_1>n$ 则只取 $k_0$）。

由于 $n\le32767$，$n/e\le12051$，用 `long double`（64 位尾数）计算 $\log$ 足够精确：两个候选值的差在最坏情况下也远大于 `long double` 的舍入误差（$n/e$ 与半整数的距离由 $e$ 的连分数逼近给出下界，量级约 $10^{-4}$，而双精度误差约 $10^{-15}$）。实现上直接用 `long double` 比较即可。

### 2.2 什么时候是有限小数

**引理 3（有理数有限小数的判别）**：既约分数 $a/b$（$\gcd(a,b)=1$）是有限小数，当且仅当 $b$ 的质因子只含 $2$ 和 $5$，即 $b=2^u5^v$。

**证明**：

- 充分性：若 $b=2^u5^v$，令 $t=\max(u,v)$，则
  $$
  \frac ab=\frac{a\cdot 2^{t-u}5^{t-v}}{10^t},
  $$
  分母为 $10^t$，是有限小数。
- 必要性：若 $a/b$ 是有限小数，设小数点后共 $t$ 位，则
  $$
  \frac ab=\frac{X}{10^t}
  $$
  对某整数 $X$。于是 $b\cdot X=a\cdot 10^t$，即 $b\mid a\cdot 10^t$。又 $\gcd(a,b)=1$，故 $b\mid 10^t=2^t5^t$，所以 $b$ 只含质因子 $2,5$。$\blacksquare$

现在
$$
M(n)=\left(\frac nk\right)^k .
$$
令 $g_0=\gcd(n,k)$，$n'=n/g_0$，$k'=k/g_0$，则 $\gcd(n',k')=1$ 且
$$
M(n)=\left(\frac{n'}{k'}\right)^k=\frac{(n')^k}{(k')^k}.
$$
因为 $\gcd(n',k')=1$，分子分母已经互质（$(n')^k$ 与 $(k')^k$ 互质）。所以 $M(n)$ 是有限小数当且仅当 $(k')^k$ 只含质因子 $2,5$，当且仅当 $k'$ 只含质因子 $2,5$。

**算法**：把 $k'$ 不断除以 $2$ 和 $5$，若最终变成 $1$ 则有限小数，否则无限小数。

验证题面例子：

- $n=8$：$n/e\approx2.943$，比较 $k=2,3$。$k=3$ 时 $(8/3)^3=512/27$，$k'=3$，含质因子 $3$，无限小数，故 $D(8)=+8$。
- $n=11$：$n/e\approx4.047$，$k=4$，$(11/4)^4=14641/256$，$k'=4=2^2$，有限小数，故 $D(11)=-11$。

与题面一致。

### 2.3 多个最优 $k$ 会不会导致 $D(n)$ 不唯一？

假设两个不同的整数 $k_1,k_2$ 给出相同的 $M(n)$。由于 $M(n)$ 是一个确定的**有理数**，“它是否为有限小数”是这个数本身的性质（引理 3 只依赖其既约分母），与用哪种写法得到无关。因此无论选哪个最优 $k$，判断结果相同，$D(n)$ 定义无歧义。

### 2.4 汇总

对 $n=5,6,\dots,a$ 依次：

1. 求 $k_0=\lfloor n/e\rfloor$，在 $k_0,k_0+1$ 中选使 $(n/k)^k$ 更大者；
2. 令 $k'=k/\gcd(n,k)$，去掉因子 $2,5$；
3. 若 $k'=1$ 则累加 $-n$，否则累加 $+n$。

得到答案。由于 $a\le32767$，答案绝对值不超过 $\sum_{n=5}^{a}n<5.4\times10^8$，但仍用 `long long` 更稳妥。

---

## 3. 模式总结（Pattern Summary）

- **continuous relaxation** — 把整数 $k$ 放松为实数，求导得到最优点 $k=n/e$。
- **unimodality** — 二阶导为负说明单峰，整数最优只在 $\lfloor n/e\rfloor$ 与 $\lceil n/e\rceil$。
- **number theory criterion** — 既约分数是有限小数 $\iff$ 分母只含质因子 $2,5$。
- **reduce the fraction** — 用 $\gcd(n,k)$ 约分，把问题化成检查 $k/\gcd(n,k)$。
- **well-definedness** — 最优值相同则小数性质相同，故无需处理并列。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| $a$ | `int` | 求和上界 | 读入 | 上界 |
| `E` | `long double` | 自然常数 $e$ | 常量 | Euler |
| `n` | `int` | 当前处理的整数 | $5\to a$ | 枚举 |
| `k0` | `int` | $\lfloor n/e\rfloor$ | 计算 | 候选 1 |
| `best` | `int` | 最优整数 $k$ | $k_0$ 或 $k_0+1$ | best k |
| `g` | `int` | $\gcd(n,k)$ | 计算 | gcd |
| `kp` | `int` | $k/\gcd(n,k)$ 去掉因子 $2,5$ 后的剩余 | 计算 | reduced denominator |
| `ans` | `i64` | 累加的 $D$ 之和 | $0$ | 答案 |

### 4.2 算法步骤

1. 读入 $a$。
2. 对 $n=5$ 到 $a$：
   - $k_0=\lfloor n/e\rfloor$（夹到 $[1,n]$）；
   - 在 $k_0$ 与 $k_0+1$（若 $\le n$）中，用 $k\ln(n/k)$ 比较，取较大者为 `best`；
   - $g=\gcd(n,\text{best})$，$kp=\text{best}/g$；
   - 反复除以 $2$、除以 $5$；
   - 若 $kp=1$ 则 `ans -= n`，否则 `ans += n`。
3. 输出 `ans`。

### 4.3 复杂度

- 时间：$O(a\log a)$（每个 $n$ 一次 $\gcd$ 与常数额外检查），$a\le32767$。
- 空间：$O(1)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1925 最大划分乘积
// https://www.luogu.com.cn/problem/P1925
#include<bits/stdc++.h>
using namespace std;

using i64=long long;

constexpr long double E{2.718281828459045235360287471352662497757L};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int a;
	cin>>a;

	i64 ans{0};
	for(int n=5;n<=a;++n)
	{
		int k0=(int)floor((long double)n/E);
		if(k0<1)k0=1;
		if(k0>n)k0=n;
		int best{k0};
		long double bv=(long double)k0*logl((long double)n/k0);
		if(k0+1<=n)
		{
			long double v=(long double)(k0+1)*logl((long double)n/(k0+1));
			if(v>bv)
			{
				bv=v;
				best=k0+1;
			}
		}

		int g=__gcd(n,best);
		int kp=best/g;
		while(kp%2==0)kp/=2;
		while(kp%5==0)kp/=5;
		if(kp==1)
			ans-=n;
		else
			ans+=n;
	}
	cout<<ans<<'\n';
	return 0;
}
```
