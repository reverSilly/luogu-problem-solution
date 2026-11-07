# P1895 数字序列

## 1. 重新读题

### 1.1 问题重述（人话版）

定义 $S_n={\tt 1+2+3+\cdots}+\mathrm{tostring}(n)$，也就是把 $1,2,\dots,n$ 的十进制写法直接首尾相接得到的字符串：

$$
S_1={\tt 1},\quad S_2={\tt 12},\quad S_3={\tt 123},\quad S_4={\tt 1234},\ \dots
$$

再令 $S_\infty=S_1+S_2+S_3+\cdots$。有 $t$ 次询问，每次给出一个位置 $i$，求 $S_\infty$ 的第 $i$ 个字符（一个十进制数字）。

### 1.2 关键约束与规模暗示

- $t\le 10$，但 $i\le 2^{31}-1\approx 2.1\times 10^9$。
- 无法把 $S_\infty$ 真的构造出来（长度十亿级），必须利用结构定位。
- 但 $t$ 很小，单次询问允许 $O(\sqrt{i})$ 甚至 $O(n)$ 的扫描，只要 $n$（块编号）足够小。
- 一个自然的分层结构：$S_\infty$ 由块 $S_1,S_2,\dots$ 拼成；先定位块，再定位块内的数字，最后定位数字中的位。关键是要证明块编号不会太大。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：三层结构

$S_\infty$ 的构造有三层嵌套：

1. **块**：$S_1,S_2,\dots$；
2. **块内的数**：$S_n={\tt 1\,2\,\cdots\,n}$，由 $n$ 个整数拼接；
3. **数内的位**：$\mathrm{tostring}(k)$ 的第几位。

因此查询也分三步：

- 先求出 $i$ 落在哪个块 $S_n$；
- 再把 $i$ 减去前面所有块的长度，得到它在 $S_n$ 内的偏移；
- 在 $S_n$ 内依次减去 $1,2,\dots$ 的位数，确定是哪个整数 $k$ 的第几位；
- 把 $k$ 的第若干位取出来。

每一层都只是「按长度做减法」，因为长度是可加的、单调的，所以定位是确定的。

### 2.2 引理 1（块的定位是唯一的）

**引理 1.** 令 $L_n=|S_n|=\sum_{k=1}^{n}\mathrm{digits}(k)$，$P_n=\sum_{m=1}^{n}L_m$。则 $P_n$ 严格递增，且对任意 $i\ge 1$，存在唯一的 $n\ge 1$ 使得

$$
P_{n-1}<i\le P_n
$$

（约定 $P_0=0$），此时第 $i$ 个字符属于且仅属于块 $S_n$，它在块内的位置为 $i-P_{n-1}$。

**证明.** 每个 $L_m\ge 1$，故 $P_n-P_{n-1}=L_n\ge 1>0$，$P_n$ 严格递增。区间 $(P_{n-1},P_n]$（整数意义下）恰好划分正整数集合，所以 $n$ 存在唯一。$S_\infty$ 是这些块的顺序拼接，第 $i$ 个字符自然落在第 $n$ 块，块内位置为 $i-P_{n-1}$。$\blacksquare$

> 代码里用的是等价的「循环减法」：维护当前累计长度 `len`，若 `i<=len` 则停，否则 `i-=len` 进入下一块。由于每轮减去的正是 $L_n$，循环结束时 `i` 就是块内位置，`n` 就是块号。

### 2.3 引理 2（块号最多约 $6.6\times 10^4$）

**引理 2.** 若 $i\le 2^{31}-1$，则包含第 $i$ 位的块号 $n$ 满足 $n\le 65536$。

**证明.** 对任意 $k\ge 1$，$\mathrm{digits}(k)\ge 1$，所以 $L_m=\sum_{k=1}^m\mathrm{digits}(k)\ge m$。于是

$$
P_{n-1}=\sum_{m=1}^{n-1}L_m\ge\sum_{m=1}^{n-1}m=\frac{n(n-1)}{2}.
$$

由引理 1，$P_{n-1}<i\le 2^{31}-1$，因此

$$
\frac{n(n-1)}{2}<2^{31}\quad\Longrightarrow\quad n<\frac{1+\sqrt{1+2^{34}}}{2}\approx 65536.5.
$$

$\blacksquare$

这个引理非常重要：它保证 **逐块扫描最多 $6.6\times 10^4$ 轮**，$t\le 10$ 时完全可行，不需要任何高深的数学加速。

### 2.4 块内定位与取位

确定块 $S_n$ 与块内位置 $i\in[1,L_n]$ 后，依次考虑 $k=1,2,\dots$：

- 若 $i\le \mathrm{digits}(k)$，说明第 $i$ 位属于整数 $k$；
- 否则 $i\mathrel{-}=\mathrm{digits}(k)$，继续看 $k+1$。

**引理 3.** 上述过程必然在某个 $k\le n$ 停下，且此时 $i$ 恰好是 $\mathrm{tostring}(k)$ 中从前往后第 $i$ 个字符的位置。

**证明.** $S_n$ 是 $1,2,\dots,n$ 的拼接，长度 $L_n=\sum_{k=1}^n\mathrm{digits}(k)\ge i$。按顺序减去各数的位数，等价于在拼接串上从左往右跳过整段 $\mathrm{tostring}(k)$。因为 $i\le L_n$，所以一定在某个 $k\le n$ 的段内停下，剩下的 $i$ 就是该段内的偏移。$\blacksquare$

取出第 $i$ 位（$1$-based）：设 $d=\mathrm{digits}(k)$，则该位后面还有 $d-i$ 位，把 $k$ 反复除以 $10$ 共 $d-i$ 次，再对 $10$ 取模即可。

### 2.5 失败尝试记录

1. **真正构造 $S_\infty$**：$i$ 到 $2\times 10^9$，内存和时间都不允许。
2. **直接求块号闭式**：$L_n$ 分段依赖位数，闭式很麻烦；而块号只有 $6.6\times 10^4$，直接扫更简单、更稳。
3. **用 `int` 存 `len` 或 `i`**：$i$ 已经贴到 `int` 上限（$2^{31}-1$），任何「先加长度再比较」「先减长度再判断」的写法都容易在边界处溢出；统一用 `long long` 最省心。
4. **取位时方向搞反**：第 $i$ 位后面剩 $d-i$ 位，除 $10^{d-i}$ 次再取模，不能直接用 $10^{i-1}$。
5. **边界 `i<=len` 写成 `i<len`**：会漏掉每块最后一个字符，也会让块号偏大。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — $S_\infty$ 是「块—数—位」三层拼接结构，长度可加且单调。
- **reduce to easier problem** — 定位问题化为按长度做减法。
- **prove a bound** — 用 $L_m\ge m$ 推出块号不超过约 $6.6\times 10^4$，从而扫描可行。
- **classic problem solution** — 两轮扫描：先找块，再找数字；最后按十进制位取字符。
- **careful with types** — 全程 `long long`。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `t` | `int` | 询问组数 | 读入 | |
| `i` | `i64` | 当前询问的位置（$1$-based 的位置，不是数组下标） | 读入 | index |
| `n` | `i64` | 当前扫描到的块号 | $0$ | block number |
| `len` | `i64` | $L_n=\lvert S_n\rvert$，块内数字位数之和 | $0$ | length |
| `dgt(x)` | 函数，返回 `int` | $x$ 的十进制位数 | — | digits |
| `k` | `i64` | 块内当前扫描到的整数 | $1$ | |
| `d` | `int` | $k$ 的位数 | 由 `dgt(k)` 得 | digits |
| `p` | `i64` | 取位前需要除 $10$ 的次数 | $d-i$ | position |

### 4.2 算法步骤

1. 读入 $t$。
2. 对每组询问读入 $i$：
   1. 初始化 `n=0, len=0`。
   2. 循环：`++n`，`len += dgt(n)`；若 `i<=len` 跳出；否则 `i -= len`。
   3. 此时 `i` 是 $S_n$ 内的 $1$-based 位置。令 `k=1`。
   4. 循环：`d=dgt(k)`；若 `i<=d`，则答案位为 $k$ 的第 $i$ 位；否则 `i-=d`，`++k`。
   5. 取位：令 `p=d-i`，把 `k` 除以 $10$ 共 `p` 次，输出 `k%10`。
3. 每个询问输出一行。

### 4.3 复杂度分析

- **单组询问**：块定位 $O(n)$，块内定位 $O(n)$，取位 $O(\log k)$；由引理 2，$n\le 65536$。
- **总时间**：$O(t\cdot 6.6\times 10^4)$，$t\le 10$ 时约 $7\times 10^5$ 次操作。
- **空间**：$O(1)$。

---

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int dgt(i64 x)
{
	int d=0;
	for(;x;x/=10)
		++d;
	return d;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int t;
	cin>>t;
	while(t--)
	{
		i64 i;
		cin>>i;
		i64 n{0},len{0};
		while(true)
		{
			++n;
			len+=dgt(n);
			if(i<=len)
				break;
			i-=len;
		}
		for(i64 k=1;;++k)
		{
			int d=dgt(k);
			if(i<=d)
			{
				i64 p=d-i;
				while(p--)
					k/=10;
				cout<<k%10<<'\n';
				break;
			}
			i-=d;
		}
	}
	return 0;
}
```
