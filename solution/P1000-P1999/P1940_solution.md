# P1940 Reversible Number

## 1. 重新读题

### 1.1 问题重述（人话版）

称正整数 $n$ 是 **Reversible** 的，如果

$$
n+\operatorname{rev}(n)
$$

的**十进制每一位都是奇数**，其中 $\operatorname{rev}(n)$ 是把 $n$ 的十进制写法倒过来得到的数。约定 $n$ 与 $\operatorname{rev}(n)$ 都**不允许有前导零**：$n$ 是正常写法（首位非零），而且 $n$ 的末位也必须非零（否则 $\operatorname{rev}(n)$ 会出现前导零）。

给定 $x$（$3\le x\le400$），求满足 $n\le 10^x$ 的 Reversible 数个数。

### 1.2 关键约束与规模暗示

- $x\le400$：答案最多有约 $300$ 位十进制，**远超 64 位**，必须高精度。
- 30% 数据答案 $\le 2^{32}-1$，但满分必须处理几百位。
- 本题改编自 Project Euler 145（原题只问 $x=9$）。这类「数位加法 + 每位奇偶」的题，正解几乎一定是**按数位长度、按位配对做组合计数**，而不是枚举。
- 已知校验值：小于 $10^3$ 有 $120$ 个；小于 $10^9$ 有 $608720$ 个。可以用它们检查公式。

---

## 2. 思维链（Chain-of-thought）

### 2.1 先写下数位加法的模型

设 $n$ 有 $d$ 位，从低到高记为 $a_0,a_1,\dots,a_{d-1}$（$a_{d-1}\ne0$ 是首位，$a_0\ne0$ 是因为末位不能为 $0$）。$\operatorname{rev}(n)$ 的第 $i$ 位就是 $a_{d-1-i}$。

做竖式加法，设 $c_i$ 是**进入第 $i$ 列（第 $i$ 位）的进位**，$c_0=0$。则第 $i$ 列满足

$$
a_i+a_{d-1-i}+c_i=m_i+10c_{i+1},\qquad c_{i+1}\in\{0,1\},
$$

其中 $m_i$ 是结果的第 $i$ 位，要求 $m_i$ 为奇数。最高位还可能产生额外进位 $c_d$；若 $c_d=1$，结果多出一位 `1`，它是奇数，允许（例：$409+904=1313$）。

### 2.2 第一个简化：奇偶只看「和」的奇偶

**引理 1（奇偶引理）**：设 $s$ 为某一列的「两个数位之和」（对于配对的列，两列共用同一个 $s$）。该列结果位为奇数，当且仅当 $s+c$ 为奇数，其中 $c$ 是进入该列的进位。

**证明**：结果位 $m=(s+c)\bmod 10$。因为 $10$ 是偶数，$(s+c)\bmod10$ 与 $s+c$ 奇偶性相同；而 $s+c\le18+1=19$，其十进制个位的奇偶就是 $s+c$ 的奇偶。$\square$

于是「每一位都是奇数」这条约束，全部变成「每一列 $s+c$ 为奇数」的**纯奇偶条件**，非常干净。

### 2.3 配对：上下两列共享同一个 $s$

对于 $i$ 和 $d-1-i$ 这一对列（两者相加的两个数位相同，只是顺序反过来），记

$$
s_i=a_i+a_{d-1-i}\quad(i<d-1-i).
$$

两列的奇偶条件分别是 $s_i+c_i$ 为奇、$s_i+c_{d-1-i}$ 为奇。两式相减（模 $2$）得

$$
c_i\equiv c_{d-1-i}\pmod 2 .
$$

而进位只取 $0/1$，所以

$$
c_i=c_{d-1-i}.
$$

这条「进位对称」是所有计数的基础。下面按 $d$ 的奇偶分两类。

### 2.4 偶数位：$d=2p$

此时列 $0,\dots,2p-1$ 两两配对，$s_i=s_{2p-1-i}$，进位条件为

$$
c_i=c_{2p-1-i}\quad(0\le i\le 2p-1).\tag{E}
$$

**引理 2（偶数位时所有进位为 0）**：在 $d=2p$ 且每位结果均为奇数的前提下，$c_0=c_1=\dots=c_{2p-1}=0$。

**证明**：

1. 对列 $2p-1-j$ 用递推式：

$$
c_{2p-j}=\Big\lfloor\frac{s_{2p-1-j}+c_{2p-1-j}}{10}\Big\rfloor
=\Big\lfloor\frac{s_j+c_j}{10}\Big\rfloor=c_{j+1},
$$

其中用了 $s_{2p-1-j}=s_j$ 与 $(E)$ 的 $c_{2p-1-j}=c_j$。
2. 在 $(E)$ 中把下标换成 $2p-k+1$（要求 $2\le k\le 2p-1$，此时下标合法）：$c_{2p-k+1}=c_{k-2}$。与第 1 步取 $j=2p-k$ 得到的 $c_k=c_{2p-k+1}$ 合并，得

$$
c_k=c_{k-2}\qquad(2\le k\le 2p-1).
$$

3. 于是所有偶数下标进位相等，且 $c_0=0$，故 $c_0=c_2=\dots=0$；所有奇数下标进位相等。又由 $(E)$ 取 $i=0$：$c_{2p-1}=c_0=0$，而 $k=2p-1$ 的关系把奇数链接到 $c_{2p-1}$，故奇数进位也为 $0$。$\square$

**计数**：进位全为 $0$ 后，条件退化为

- 每列 $s_j+c_j=s_j$ 为奇数，即 $s_j$ 为奇数；
- $c_{j+1}=\lfloor s_j/10\rfloor=0$，即 $s_j\le9$。

所以每个配对和 $s_j\in\{1,3,5,7,9\}$。

- $j=0$ 是最外一对 $(a_0,a_{2p-1})$，两数位都非零（末位、首位均非零），$s_0$ 是奇数且 $\ge2$，可行值为 $3,5,7,9$。数对 $(a,b)\in[1,9]^2$ 中和为 $s$ 的个数是 $s-1$，合计
  $$2+4+6+8=20 .$$
- 内部配对 $j=1,\dots,p-1$：数位可含 $0$，$(a,b)\in[0,9]^2$ 中和为 $s$ 的个数是 $s+1$，合计
  $$2+4+6+8+10=30 .$$

因此偶数位（$d=2p$）的个数为

$$
\boxed{E_p=20\cdot 30^{\,p-1}}\qquad(p\ge1).
$$

校验：$d=2$ 得 $20$，$d=4$ 得 $600$，$d=6$ 得 $18000$，$d=8$ 得 $540000$，与暴力/PE145 一致。

### 2.5 奇数位：$d=2p+1$

列 $0,\dots,2p$，配对是 $(i,2p-i)$，$i=0,\dots,p-1$；第 $p$ 列是中间列，两个数位都是 $a_p$，和为 $2a_p$。

对称条件变为

$$
c_i=c_{2p-i}\quad(0\le i\le p-1),\tag{O}
$$

特别地 $c_0=c_{2p}=0$。

**中间列**：需要 $2a_p+c_p$ 为奇数 $\Rightarrow c_p=1$。于是中间列输出位 $m_p=(2a_p+1)\bmod10$ 必为奇数（任意 $a_p$ 都行），而

$$
c_{p+1}=\Big\lfloor\frac{2a_p+1}{10}\Big\rfloor=\begin{cases}0,&a_p\le4\\1,&a_p\ge5\end{cases}
$$

**对称链**：对高位第 $p+k$ 列（$1\le k\le p-1$），其配对和是 $s_{p-k}$，进入进位由 $(O)$ 是 $c_{p+k}=c_{p-k}$，故

$$
c_{p+k+1}=\Big\lfloor\frac{s_{p-k}+c_{p-k}}{10}\Big\rfloor=c_{p-k+1}.
$$

另一方面 $(O)$ 要求 $c_{p+k+1}=c_{p-k-1}$，于是

$$
c_{p-k+1}=c_{p-k-1}\quad\Longrightarrow\quad c_{j+2}=c_j\ (0\le j\le p-2).
$$

再由 $k=p-1$ 得 $c_{2p}=c_2$，而 $c_{2p}=c_0=0$，故 $c_2=0$。

所以：**所有偶数下标的进位相等且等于 $c_0=0$；所有奇数下标的进位相等，记作 $u$**。

**$p$ 必须是奇数**：

- 若 $p$ 为偶数，则 $c_p=0$（偶数链），与中间列要求的 $c_p=1$ 矛盾；
- 若 $p$ 为奇数，则 $c_p=u$，结合 $c_p=1$ 得 $u=1$，即奇数下标进位全为 $1$。

因此 $p$ 为偶数时该长度**没有任何解**（例如 $5$ 位、$9$ 位数均为 $0$ 个，与暴力一致）。

设 $p$ 为奇数。由 $c_{p+1}=c_{p-1}=0$（$p-1$ 为偶数）与中间列公式，得到中间数字 $a_p\le4$，共 $5$ 种选择。

低半部分的每个配对 $j=0,\dots,p-1$：

- $j$ 为偶数：$c_j=0,\ c_{j+1}=1$，故 $s_j\ge10$；又 $s_j+c_j=s_j$ 为奇数，所以 $s_j\in\{11,13,15,17\}$；
- $j$ 为奇数：$c_j=1,\ c_{j+1}=0$，故 $s_j+1\le9$；又 $s_j+1$ 为奇数，所以 $s_j$ 为偶数，$s_j\in\{0,2,4,6,8\}$。

计数：

- 偶数 $j$（含 $j=0$）：$s\in\{11,13,15,17\}$ 的数对个数为 $8+6+4+2=20$（$s\ge11$ 时数位不可能为 $0$，所以最外一对「两数位非零」不额外减少计数）；
- 奇数 $j$：$s\in\{0,2,4,6,8\}$ 的数对个数为 $1+3+5+7+9=25$。

在 $j=0,\dots,p-1$ 中，偶数 $j$ 有 $(p+1)/2$ 个，奇数 $j$ 有 $(p-1)/2$ 个。故

$$
\boxed{O_p=\begin{cases}5\cdot 20^{\frac{p+1}{2}}\cdot 25^{\frac{p-1}{2}},&p\text{ 为奇数},\\[4pt]0,&p\text{ 为偶数}.\end{cases}}
$$

等价地，$O_p=100\cdot 500^{\frac{p-1}{2}}$（$p$ 为奇数）。校验：$d=3$（$p=1$）得 $100$；$d=7$（$p=3$）得 $50000$。

### 2.6 汇总

$$
\text{answer}(x)=\sum_{p=1}^{\lfloor x/2\rfloor}20\cdot30^{p-1}
+\sum_{\substack{p\ \text{odd}\\2p+1\le x}}5\cdot20^{\frac{p+1}{2}}\cdot25^{\frac{p-1}{2}}.
$$

$x=3$：$20+100=120$ ✓；$x=9$：$558620+50100=608720$ ✓（与 PE145 一致）。

### 2.7 「$\le10^x$」与歧义的说明

- 题面写「小于等于 $10^x$」，PE 原文是「below one-billion」。两者在本问题中**结果相同**：$10^x$ 的十进制是 `1` 后跟 $x$ 个 `0`，末位为 $0$，$\operatorname{rev}(10^x)=1$，$10^x+1$ 的中间全是偶数字 $0$，所以 $10^x$ 本身不是 Reversible 数。故「$\le$」与「$<$」等价，我们按「所有位数 $\le x$ 的 $n$」来数。
- 「以 $0$ 开头不算」我们解释为：$n$ 的正常十进制写法首位非零，且 $\operatorname{rev}(n)$ 不能出现前导零，即 $n$ 的末位非零。两者在计数中都落实为「最外一对数位均非零」。

### 2.8 高精度

答案最多约 $296$ 位十进制，用「以 $10^9$ 为基」的高精度：只做「乘一个小整数」「加」两种运算。每一轮偶数项乘 $30$，奇数项乘 $500$，边累加边推进，复杂度与位数成正比。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 竖式加法建模，把「结果每位为奇数」化成「每列 $s+c$ 为奇」的纯奇偶条件。
- **explore its property** — 配对的上下两列共享同一个数位和，推出进位对称 $c_i=c_{d-1-i}$。
- **discuss by category** — 按位数奇偶分类：偶数位证出进位全 $0$；奇数位证出进位必须奇偶交替，且 $p$ 必须为奇数。
- **classic problem solution** — 逐对独立计数（每对只有数位和的选择），得到闭式 $20\cdot30^{p-1}$ 与 $5\cdot20^{(p+1)/2}25^{(p-1)/2}$。
- **classic problem solution** — 以 $10^9$ 为基的高精度，只做「乘小整数 + 加」累加答案。

---

## 4. 真实解法（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `x` | `int` | 输入上界 $10^x$ | 读入 | 位数上界 |
| `BASE` | `constexpr int` | 高精度基 $10^9$ | — | base |
| `big.d[]`, `big.len` | `int [64]`, `int` | 高精度数，低位在前，`BASE=10^9` | `fromInt` | 大整数 |
| `ans` | `big` | 答案累加器 | 0 | answer |
| `ev` | `big` | 当前偶数位的个数 $20\cdot30^{p-1}$ | 20 | even |
| `od` | `big` | 当前奇数位的个数 $100\cdot500^{(p-1)/2}$ | 100 | odd |

### 4.2 算法步骤

1. 读入 $x$。
2. `ans=0`；`ev=20`。对 $p=1,2,\dots,\lfloor x/2\rfloor$：`ans += ev`，然后 `ev *= 30`。
3. `od=100`。对 $p=1,3,5,\dots$ 且 $2p+1\le x$：`ans += od`，然后 `od *= 500`。
4. 输出 `ans`（高精度十进制，最高位不补零、其余每 $9$ 位补足 $9$ 位）。

### 4.3 复杂度分析

- **时间复杂度**：循环次数 $O(x)$，每次高精度乘/加 $O(x)$ 个「基位」，合计 $O(x^2)$，$x\le400$ 时约 $10^5$ 次基本运算。
- **空间复杂度**：$O(x)$ 个基位，即常数大小的固定数组。

---

## 5. 示例代码

```cpp
//
// 题目：P1940 Reversible Number
// https://www.luogu.com.cn/problem/P1940
#include<bits/stdc++.h>
using namespace std;

constexpr int BASE{1000000000};
constexpr int mxn{64};

struct big
{
	int d[mxn];
	int len;
};

big fromInt(int v)
{
	big r{};
	do
	{
		r.d[r.len++]=v%BASE;
		v/=BASE;
	}while(v>0);
	return r;
}

big add(big a,big b)
{
	big r{};
	int n=max(a.len,b.len);
	long long carry=0;
	for(int i=0;i<n;++i)
	{
		long long cur=carry;
		if(i<a.len)
			cur+=a.d[i];
		if(i<b.len)
			cur+=b.d[i];
		r.d[i]=cur%BASE;
		carry=cur/BASE;
	}
	r.len=n;
	if(carry)
		r.d[r.len++]=carry;
	return r;
}

big mul(big a,int k)
{
	big r{};
	long long carry=0;
	for(int i=0;i<a.len;++i)
	{
		long long cur=(long long)a.d[i]*k+carry;
		r.d[i]=cur%BASE;
		carry=cur/BASE;
	}
	r.len=a.len;
	while(carry)
	{
		r.d[r.len++]=carry%BASE;
		carry/=BASE;
	}
	return r;
}

void print(const big&a)
{
	cout<<a.d[a.len-1];
	for(int i=a.len-2;i>=0;--i)
		cout<<setw(9)<<setfill('0')<<a.d[i];
	cout<<'\n';
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int x;
	cin>>x;

	big ans=fromInt(0);
	big ev=fromInt(20);
	for(int p=1;2*p<=x;++p)
	{
		ans=add(ans,ev);
		ev=mul(ev,30);
	}
	big od=fromInt(100);
	for(int p=1;2*p+1<=x;p+=2)
	{
		ans=add(ans,od);
		od=mul(od,500);
	}
	print(ans);
	return 0;
}
```
