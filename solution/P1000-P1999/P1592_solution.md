# P1592 互质

## Part 1 — 题目理解 (Problem Comprehension)

### 题意简述

给定 $n$ 和 $k$，把与 $n$ 互质的正整数按从小到大排成一列 $1=c_1<c_2<\cdots$（这里 $1$ 与任何数互质），求第 $k$ 个。

### 输入输出格式

- **输入格式**：一行两个正整数 $n,k$。
- **输出格式**：一行一个正整数，即第 $k$ 个与 $n$ 互质的数。
- **数据规模**：$1\le n\le10^6$，$1\le k\le10^8$。

### 关键约束分析

1. $k$ 可达 $10^8$，答案的量级是 $k\cdot\dfrac{n}{\varphi(n)}$，最坏约 $10^8\times10^6=10^{14}$，**答案必须用 `long long`**。
2. 不能逐个枚举判断：$10^{14}$ 次 `gcd` 必然超时。必须利用互质关系的**周期性**一次跳过大段。
3. $n=1$ 是边界：所有正整数都与 $1$ 互质，答案是 $k$。此时 $\varphi(1)=1$，公式必须自洽。
4. 单组数据，无需考虑多测清空。

---

## Part 2 — 思路推导 (Idea Discovery)

### 第一步：暴力及其瓶颈

从 $1$ 开始逐个判断 $\gcd(x,n)=1$，数到第 $k$ 个。$k\le10^8$ 且答案可能是 $10^{14}$，完全不可行。瓶颈在于：判断每个 $x$ 都要做一次 $\gcd$，而 $x$ 的取值极度冗余——它们按 $n$ 循环重复。

### 第二步：发现周期性

**引理 1（平移不变）**：对任意整数 $x$ 与 $b\ge0$，$\gcd(x+bn,n)=\gcd(x,n)$。

*证明*：$\gcd(a,n)$ 只依赖 $a \bmod n$。由辗转相除的整除性，$n \mid (x+bn)-x$，故 $x+bn\equiv x\pmod n$，两者与 $n$ 的最大公因数相同。更具体地，$d\mid n$ 时 $d\mid x+bn \iff d\mid x$，所以两者的公因数集合完全相同。$\square$

于是"是否与 $n$ 互质"这个性质以 $n$ 为周期重复：第 $b$ 个周期（$bn+1$ 到 $(b+1)n$）内的互质数恰好是第 $0$ 个周期内的互质数整体平移 $bn$。

**引理 2（每周期个数）**：在一个完整周期 $\{1,2,\dots,n\}$ 中，与 $n$ 互质的数恰有 $\varphi(n)$ 个。

*证明*：这是欧拉函数的定义：$\varphi(n)=\#\{1\le x\le n:\gcd(x,n)=1\}$。注意 $x=n$ 与 $n$ 不互质（$n>1$），$x=n$ 只在 $n=1$ 时等于 $1$，此时 $\gcd(1,1)=1$ 计数为 $1=\varphi(1)$，定义一致。$\square$

### 第三步：把第 $k$ 个定位到"第几周期 + 周期内第几个"

设周期内互质数升序为 $c_0<c_1<\cdots<c_{\varphi(n)-1}$（代码 0-based，$c_0=1$）。

**引理 3（全局排名公式）**：第 $k$ 个与 $n$ 互质的正整数为

$$q\cdot n+c_r,\qquad q=\left\lfloor\frac{k-1}{\varphi(n)}\right\rfloor,\quad r=(k-1)\bmod\varphi(n).$$

*证明*：由引理 1，所有互质数可写成 $b n+c_i$（$b\ge0$，$0\le i<\varphi$），且不同 $(b,i)$ 给出不同的数。比较两个数：若 $b_1<b_2$，则 $b_1n+c_{i_1}\le b_1n+n\le b_2n<b_2n+c_{i_2}$（这里用 $c_i\le n$），故按 $b$ 分块后块间严格递增；块内按 $c_i$ 递增。于是第 $k$ 个（1-based）落在第 $q=\lfloor(k-1)/\varphi\rfloor$ 块、块内第 $r+1$ 个，即 $qn+c_r$。$\square$

**定理**：先求 $\varphi(n)$，再枚举 $[1,n]$ 内的互质数存入数组，最后按引理 3 取值，即得答案。

*证明*：引理 2 保证枚举出的个数就是 $\varphi(n)$，引理 3 保证下标定位正确。$\square$

### 第四步：$\varphi(n)$ 怎么求

$n\le10^6$，直接试除到 $\sqrt n$：若 $p\mid n$ 则 $\varphi\gets\varphi/p\cdot(p-1)$，并把 $p$ 除干净；最后若剩余 $m>1$，说明还有一个大素因子，同样乘 $(m-1)/m$。这一步 $O(\sqrt n)$。

### 第五步：枚举周期内的互质数

直接 `for(i=1..n) if(gcd(i,n)==1) cop[cnt++]=i`，$n\le10^6$，一次 `gcd` 是 $O(\log n)$，总约 $2\times10^7$ 次基本操作，足够快。也可以只用 $\varphi(n)$ 与容斥跳过，但没有必要。

---

## Part 3 — 模式总结 (Pattern Summary)

- `explore its property` — 互质性只依赖 $x\bmod n$，序列以 $n$ 为周期（引理 1）。
- `classic problem solution` — 用欧拉函数 $\varphi(n)$ 数出每个周期的个数（引理 2）。
- `reduce to easier problem` — 大排名 $k$ 拆成"第 $q$ 个周期 + 周期内第 $r$ 个"，把 $10^8$ 的枚举降成 $10^6$ 的预处理。
- `classic problem solution` — 试除法 $O(\sqrt n)$ 求 $\varphi(n)$。
- `prove by block ordering` — 用块间/块内的双重递增证明排名公式无歧义。

---

## Part 4 — 真正的解法 (The Real Solution)

### 符号表 (Notation Glossary)

| 数学符号 | 代码变量 | 含义说明 |
| :--- | :--- | :--- |
| $n$ | `n` | 模数，也是周期长度 |
| $k$ | `k` | 要求的排名（1-based） |
| $\varphi(n)$ | `phi` | 一个周期内互质数个数；试除求得，兼作取模的除数 |
| $m$ | `m` | 试除时的剩余因子，初值 $n$，用于判断是否还剩一个大素因子 |
| $p$ | `p` | 试除的候选素因子 |
| $c_i$ | `cop[i]` | 周期 $[1,n]$ 内第 $i$ 个（0-based）与 $n$ 互质的数，升序 |
| $\text{cnt}$ | `cnt` | 已收集的互质数个数，最终应等于 `phi` |
| $q$ | `q` | 完整周期个数 $\lfloor(k-1)/\varphi\rfloor$ |
| $r$ | `r` | 周期内偏移 $(k-1)\bmod\varphi$ |
| $\text{ans}$ | `ans` | 答案 $qn+\text{cop}[r]$，`i64` |

### 算法步骤

1. 读入 `n,k`。
2. 令 `phi=n`、`m=n`，对 $p=2,3,\dots$ 且 $p^2\le m$ 试除：若 `m%p==0` 则 `phi=phi/p*(p-1)`，并把 `m` 中的因子 `p` 除尽。
3. 若 `m>1`，再令 `phi=phi/m*(m-1)`。
4. 枚举 `i=1..n`，若 `gcd(i,n)==1` 则 `cop[cnt++]=i`。
5. `q=(k-1)/phi`，`r=(k-1)%phi`，输出 `(i64)q*n+cop[r]`。

### 复杂度分析

- **时间复杂度**：试除 $O(\sqrt n)$，枚举互质数 $O(n\log n)$（每次 `gcd`），定位 $O(1)$；总计 $O(n\log n)\le$ 约 $2\times10^7$ 次操作。
- **空间复杂度**：$O(n)$，`cop` 数组最多存 $\varphi(n)\le n$ 个数。

### 正确性验证

引理 1 给出周期性，引理 2 给出每周期个数 $\varphi(n)$，引理 3 给出排名公式，定理保证算法输出正确。数值安全：`q*n` 最大约 $10^{14}$，用 `i64`；`phi>=1`（$n=1$ 时为 $1$），不会除零；`cop[r]` 中 $r<\varphi(n)=\text{cnt}$，下标安全。

---

## Part 5 — 示例代码 (Example Code)

```cpp
//
// 题目：P1592 互质
// https://www.luogu.com.cn/problem/P1592
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int n,k;
int cop[1000005];               // cop[i]: 第 i 个与 n 互质的数(在 [1,n] 内, 0-based)
int cnt;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;

	int phi=n;                  // 试除求欧拉函数
	int m=n;
	for(int p=2;(i64)p*p<=m;++p)
		if(m%p==0)
		{
			phi=phi/p*(p-1);
			while(m%p==0)
				m/=p;
		}
	if(m>1)
		phi=phi/m*(m-1);

	for(int i=1;i<=n;++i)
		if(gcd(i,n)==1)
			cop[cnt++]=i;

	int q=(k-1)/phi,r=(k-1)%phi;    // 周期性: 每 n 个整数恰有 phi 个互质
	i64 ans=(i64)q*n+cop[r];
	cout<<ans<<'\n';
	return 0;
}
```

### 易错点

1. **答案用 `int`**：$q\cdot n$ 可达 $10^{14}$，必须 `i64`（`(i64)q*n` 的强制转换不能省，否则先按 `int` 乘再转已溢出）。
2. **忘记 $\varphi(1)=1$**：`phi` 初值写 `n` 时 $n=1$ 得 $1$，恰好正确；若写成 `n-1` 或特殊处理反而错。
3. **试除边界写错**：条件是 `p*p<=m`（随 `m` 缩小），循环结束后若 `m>1` 还要再乘一次。
4. **`cop[r]` 的下标**：`r` 是 $(k-1)\bmod\varphi$，0-based，正好取 `cop[r]`；用 `k%phi` 会错位一位。
5. **枚举时漏掉 $i=n$**：$n=1$ 时唯一的互质数是 $1$，循环上界必须是 `i<=n`。

### 本地验证结果

题面未附样例数据，使用暴搜对拍。

| 测试项 | 覆盖范围 | 结果 |
| :--- | :--- | :--- |
| 题面样例 | 题面未提供样例 | 无 |
| 手测用例 | `12 3 → 7`（$1,5,7,11$ 中第 $3$ 个） | 通过 |
| 边界用例 | `1 100000000 → 100000000`、`2 1 → 1`、`2 5 → 9`（$1,3,5,7,9$） | 通过 |
| 随机对拍 | $205$ 组（$n\le60,k\le400$ 共 $150$ 组；$n\in[999900,10^6],k\le300$ 共 $50$ 组；另含 $5$ 组固定边界），对照 `P1592_brute.cpp`（逐个 `gcd` 数到第 $k$ 个） | $205$ 组全部一致 |
