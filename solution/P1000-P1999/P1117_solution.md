# P1117 [NOI2016] 优秀的拆分 题解

---

## 1. 重新阅读题目

给定一个长度为 $n$ 的字符串 $S$。如果一个字符串可以被拆分为 $\text{AABB}$ 的形式（$\text{A},\text{B}$ 均为**非空**字符串，$\text{A}$ 和 $\text{B}$ 可以相等），则称这种拆分是**优秀的**。

我们需要求出：$S$ 的**所有子串**的所有拆分方式中，优秀拆分的**总个数**。

关键点：
- 出现在不同位置的相同子串，视为不同子串，分别计入答案。
- 同一个子串的不同优秀拆分方式（例如不同的 $\text{A},\text{B}$ 选择），也分别计入答案。
- 子串是连续的。
- 允许 $\text{A} = \text{B}$。
- $T \le 10$ 组数据，每组 $n \le 30000$，仅含小写字母。

**输出**：每组数据一个整数，表示优秀拆分总数。

---

## 2. 思维链——从暴力到正解，包含完整推导与证明

### 2.1 问题转化

一个 $\text{AABB}$ 拆分，本质上是在子串中间某个位置切开，前半段是 $\text{AA}$（即两个相同的非空字符串 $\text{A}$ 拼接），后半段是 $\text{BB}$（即两个相同的非空字符串 $\text{B}$ 拼接）。

设：
- $pre[i]$ = 以位置 $i$ 结尾的 $\text{AA}$ 形式子串的数量（$i$ 是 $\text{AA}$ 的最后一个字符的下标）。
- $suf[i]$ = 以位置 $i$ 开头的 $\text{AA}$ 形式子串的数量（$i$ 是 $\text{AA}$ 的第一个字符的下标）。

**引理 1**：答案等于 $\displaystyle \sum_{i=0}^{n-2} pre[i] \times suf[i+1]$。

**证明**：考虑任意一个 $\text{AABB}$ 拆分。设 $\text{AA}$ 部分占据 $[l, m]$，$\text{BB}$ 部分占据 $[m+1, r]$。那么：
- $\text{AA}$ 是一个以 $m$ 结尾的平方（$\text{AA}$ 形式子串），被 $pre[m]$ 计数一次。
- $\text{BB}$ 是一个以 $m+1$ 开头的平方（$\text{AA}$ 形式子串），被 $suf[m+1]$ 计数一次。
- 反之，任意一对 $(\text{AA}$ 以 $i$ 结尾$, \text{AA}$ 以 $i+1$ 开头$)$ 唯一对应一个 $\text{AABB}$ 拆分（拼接即得）。
- 不同 $\text{AABB}$ 的拆分位置不同，或同一拆分位置但 $\text{A}$ 不同/$\text{B}$ 不同，均被不同的 $(pre[i], suf[i+1])$ 对唯一计数。

因此问题完全转化为：**如何高效计算 $pre[i]$ 和 $suf[i]$？**

### 2.2 平方子串的计数——调和级数枚举法

我们称形如 $\text{XX}$（两个相同非空串拼接）的子串为**平方子串**。设平方子串的总长度为 $2L$，其中 $L = |X|$ 为周期。

**核心观察**：对于固定的周期长度 $L$，所有周期为 $L$ 的平方子串可以**批量**处理。具体做法是在字符串上每隔 $L$ 放置一个**关键点**，检查相邻关键点之间的匹配关系。

**算法框架**（对每个 $L = 1 \ldots \lfloor n/2 \rfloor$）：

1. 在位置 $0, L, 2L, 3L, \ldots$ 放置关键点。
2. 对每对相邻关键点 $(q, p) = ((k-1)L,\; kL)$：
   - 计算从 $q$ 和 $p$ 开始往右的**最长公共前缀**长度，记为 $lcp$（上限 $L$）。
   - 计算从 $q-1$ 和 $p-1$ 开始往左的**最长公共后缀**长度，记为 $lcs$（上限 $L$，若 $q=0$ 则 $lcs=0$）。
   - 若 $lcp + lcs \ge L$，则存在周期为 $L$ 的平方子串跨越这对关键点。

**正确性证明**：

设 $q = (k-1)L$，$p = kL$。考虑一个周期为 $L$ 的平方子串 $S[s, s+2L-1]$（即 $\text{XX}$，$|X|=L$），它的第一个 $\text{X}$ 占据 $[s, s+L-1]$，第二个 $\text{X}$ 占据 $[s+L, s+2L-1]$。两个 $\text{X}$ 的边界位于 $s+L-1$ 和 $s+L$ 之间。

> **关键断言**：一个周期为 $L$ 的平方子串的边界必然落在某对相邻关键点之间。

因为 $q$ 和 $p = q+L$ 是相邻关键点，且所有间隔均为 $L$，所以任意长度为 $L$ 的区间必定恰好包含一个关键点。平方子串的边界（$s+L-1$ 与 $s+L$）间距为 $1$，不可能完整跨越两个关键点间隔，因此它必被某对 $(q, p)$ 捕捉。

**区间参数化**：对于这对关键点 $(q, p)$，令 $s = q - t$，其中 $t$ 为偏移量。那么：
- 平方子串起点为 $s = q - t$
- 平方子串终点为 $s + 2L - 1 = q - t + 2L - 1$

现在我们需要 $S[q-t, q-t+L-1] = S[p-t, p-t+L-1]$。

由 $lcp$ 定义：$S[q, q+lcp-1] = S[p, p+lcp-1]$。
由 $lcs$ 定义：$S[q-lcs, q-1] = S[p-lcs, p-1]$。

于是对于 $t \in [0, lcs]$，有 $S[q-t, q-1] = S[p-t, p-1]$（向左匹配 $t$ 步）。结合 $S[q, q+lcp-1] = S[p, p+lcp-1]$，得到：
$$S[q-t, q+lcp-1] = S[p-t, p+lcp-1]$$
匹配长度为 $lcp + t$。

平方子串要求匹配长度至少为 $L$，即：
$$lcp + t \ge L \;\Longrightarrow\; t \ge L - lcp$$

同时 $t$ 受限于：
- $t \le lcs$（向左匹配的上限）
- $t \le L-1$（因为 $s \ge q - L + 1$，平方子串起点不能越过前一个关键点）
- $t \le q$（$s \ge 0$）

因此有效 $t$ 的范围为：
$$t_{\min} = \max(0,\; L - lcp),\quad t_{\max} = \min(lcs,\; L - 1,\; q)$$

当 $t_{\min} \le t_{\max}$ 时，每个 $t$ 对应一个合法的平方子串。

### 2.3 后缀数组与 $\text{LCP}/\text{LCS}$ 查询

我们需要快速查询任意两个后缀的 $\text{LCP}$ 和任意两个前缀的 $\text{LCS}$。

**后缀数组 $\text{SA}$**：将 $S$ 的所有后缀排序。$sa[i]$ 表示排名第 $i$ 的后缀的起始位置。$rk[i]$ 表示起始位置 $i$ 的后缀的排名。$height[i] = \text{LCP}(sa[i], sa[i-1])$。

**$\text{LCP}$ 查询**：$\text{LCP}(i, j) = \min\limits_{rk[i] < k \le rk[j]} height[k]$（假设 $rk[i] < rk[j]$），可用 ST 表实现 $O(1)$ 查询。

**$\text{LCS}$ 查询**：$\text{LCS}(i, j)$（$S[0..i]$ 和 $S[0..j]$ 的最长公共后缀）等价于对**反转字符串** $revS$ 做 $\text{LCP}$：
$$\text{LCS}_S(i, j) = \text{LCP}_{revS}(n-1-i,\; n-1-j)$$
因此我们构建两个后缀数组：一个对原串 $S$，一个对反转串 $revS$。

**复杂度**：SA 构建 $O(n \log n)$，ST 表 $O(n \log n)$，每次查询 $O(1)$。

### 2.4 整体算法流程

1. 读入 $S$，长度 $n$。
2. 构建 $S$ 的 SA（用于 LCP 查询）。
3. 构建 $revS$ 的 SA（用于 LCS 查询，LCS 在原串中对应 LCP 在反转串中）。
4. 初始化差分数组 $pd[]$ 和 $sd[]$，分别用于 $pre$ 和 $suf$ 的区间加。
5. 枚举平方子串周期长度 $L = 1 \ldots \lfloor n/2 \rfloor$：
   - 枚举关键点 $q = 0, L, 2L, \ldots$，令 $p = q + L$，若 $p \ge n$ 则跳出。
   - 计算 $lcp = \min(L, \text{LCP}(q, p))$。
   - 计算 $lcs = q > 0 \;?\; \min(L, \text{LCS}(q-1, p-1)) : 0$。
   - 若 $lcp + lcs \ge L$：
     - $lo = \max(0, L - lcp)$
     - $hi = \min(lcs, L - 1, q)$
     - 若 $lo \le hi$：
       - 平方子串起点范围：$[q - hi, q - lo]$
       - 平方子串终点范围：$[q - hi + 2L - 1,\; q - lo + 2L - 1]$
       - 更新差分：$pd[end\_l] \mathbin{+}= 1,\; pd[end\_r + 1] \mathbin{-}= 1$
       - 更新差分：$sd[start\_l] \mathbin{+}= 1,\; sd[start\_r + 1] \mathbin{-}= 1$
6. 对 $pd$ 和 $sd$ 做前缀和得到 $pre[i]$ 和 $suf[i]$。
7. 计算答案 $ans = \sum_{i=0}^{n-2} pre[i] \times suf[i+1]$。

### 2.5 复杂度分析

- SA 构建：$O(n \log n)$
- ST 表构建：$O(n \log n)$
- 调和级数枚举：$\displaystyle \sum_{L=1}^{n/2} \frac{n}{L} = O(n \log n)$
- 总复杂度：$O(n \log n)$，对 $T$ 组数据总复杂度 $O(\sum n \log n)$，$n \le 30000$ 时完全可行。

---

## 3. 模式总结

| 核心思想 | 说明 |
|:---|:---|
| **分界计数** | 将 $\text{AABB}$ 按 $\text{AA}$ 与 $\text{BB}$ 之间的边界拆分为两个独立计数问题 |
| **平方子串** | $\text{AA}$ 本身就是特殊子串（平方），通过枚举周期 $L$ 和关键点批量处理 |
| **调和级数枚举** | 对每个 $L$ 放 $\frac{n}{L}$ 个关键点，总复杂度 $\sum \frac{n}{L} = O(n \log n)$ |
| **LCP/LCS 判定** | 用后缀数组 + ST 表 $O(1)$ 判定两个位置的匹配长度 |
| **差分数组** | 平方子串的起点和终点构成连续区间，用差分数组做区间加，最后前缀和还原 |

---

## 4. 标准解法与符号说明

### 符号表

| 符号 | 含义 |
|:---|:---|
| $n$ | 字符串长度 |
| $S$ | 原字符串，0-indexed |
| $revS$ | 反转字符串，$revS[i] = S[n-1-i]$ |
| $sa[]$ | 后缀数组，$sa[i]$ 为排名 $i$ 的后缀起点 |
| $rk[]$ | 排名数组，$rk[i]$ 为起点 $i$ 的后缀的排名 |
| $height[]$ | $height[i] = \text{LCP}(sa[i], sa[i-1])$ |
| $\text{LCP}(i,j)$ | 后缀 $S[i..]$ 和 $S[j..]$ 的最长公共前缀长度 |
| $\text{LCS}(i,j)$ | 前缀 $S[0..i]$ 和 $S[0..j]$ 的最长公共后缀长度 |
| $L$ | 平方子串的周期，即 $|X|$ |
| $pre[i]$ | 以 $i$ 结尾的平方子串个数 |
| $suf[i]$ | 以 $i$ 开头的平方子串个数 |

### 解法步骤

1. **建 SA**：对原串 $S$ 建后缀数组，得到 $height$ 数组，建 ST 表。
2. **建反转 SA**：对 $revS$ 同样建后缀数组和 ST 表。
3. **枚举 $L$**：对 $L = 1 \ldots \lfloor n/2 \rfloor$，枚举关键点对 $(q, p) = ((k-1)L, kL)$。
4. **计算区间**：对每对关键点，计算 $lcp, lcs$，得出 $t$ 的合法范围 $[lo, hi]$。进而得到平方子串的起点区间和终点区间。
5. **差分更新**：对起点区间和终点区间分别做差分数组的区间加。
6. **前缀和还原**：对差分数组做前缀和，得到 $pre$ 和 $suf$。
7. **求和**：$ans = \sum pre[i] \cdot suf[i+1]$。

---

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{30005};
constexpr int lgn{16};

struct SA
{
	int n;
	char *s;
	int sa[mxn], rk[mxn], height[mxn];
	int st[mxn][lgn];

	void build(int _n, char *_s)
	{
		n = _n;
		s = _s;
		// ---- suffix array (doubling) ----
		int m = 128;
		static int cnt[mxn], tmp[mxn];

		fill_n(cnt, m, 0);
		for (int i = 0; i < n; i++)
			cnt[rk[i] = s[i]]++;
		for (int i = 1; i < m; i++)
			cnt[i] += cnt[i-1];
		for (int i = n-1; i >= 0; i--)
			sa[--cnt[rk[i]]] = i;

		for (int w = 1; w < n; w <<= 1)
		{
			int p = 0;
			for (int i = n-w; i < n; i++)
				tmp[p++] = i;
			for (int i = 0; i < n; i++)
				if (sa[i] >= w)
					tmp[p++] = sa[i] - w;

			fill_n(cnt, m, 0);
			for (int i = 0; i < n; i++)
				cnt[rk[tmp[i]]]++;
			for (int i = 1; i < m; i++)
				cnt[i] += cnt[i-1];
			for (int i = n-1; i >= 0; i--)
				sa[--cnt[rk[tmp[i]]]] = tmp[i];

			swap(rk, tmp);
			rk[sa[0]] = 0;
			m = 1;
			for (int i = 1; i < n; i++)
			{
				int x = sa[i], y = sa[i-1];
				rk[x] = (tmp[x] == tmp[y] &&
					(x+w < n ? tmp[x+w] : -1) == (y+w < n ? tmp[y+w] : -1))
					? m-1 : m++;
			}
			if (m == n)
				break;
		}

		// ---- height array ----
		int k = 0;
		for (int i = 0; i < n; i++)
		{
			if (rk[i] == 0)
			{
				k = 0;
				continue;
			}
			if (k)
				k--;
			int j = sa[rk[i]-1];
			while (i+k < n && j+k < n && s[i+k] == s[j+k])
				k++;
			height[rk[i]] = k;
		}

		// ---- ST table for RMQ ----
		for (int i = 0; i < n; i++)
			st[i][0] = height[i];
		for (int j = 1; (1<<j) <= n; j++)
			for (int i = 0; i + (1<<j) - 1 < n; i++)
				st[i][j] = min(st[i][j-1], st[i+(1<<(j-1))][j-1]);
	}

	int lcp(int i, int j)
	{
		if (i == j)
			return n - i;
		i = rk[i];
		j = rk[j];
		if (i > j)
			swap(i, j);
		i++;
		int k = __lg(j - i + 1);
		return min(st[i][k], st[j - (1<<k) + 1][k]);
	}
};

int T, n;
char s[mxn], rs[mxn];
SA sa, rsa;
i64 pd[mxn], sd[mxn];

int lcs(int i, int j)
{
	if (i < 0 || j < 0)
		return 0;
	return rsa.lcp(n-1-i, n-1-j);
}

i64 solve()
{
	n = strlen(s);
	// build SA for S
	sa.build(n, s);
	// build SA for reversed S
	for (int i = 0; i < n; i++)
		rs[i] = s[n-1-i];
	rsa.build(n, rs);

	fill_n(pd, n+1, 0);
	fill_n(sd, n+1, 0);

	for (int L = 1; L <= n/2; L++)
	{
		for (int q = 0; q + L < n; q += L)
		{
			int p = q + L;
			int lcp = min(L, sa.lcp(q, p));
			int lcs_len = q > 0 ? min(L, lcs(q-1, p-1)) : 0;

			if (lcp + lcs_len < L)
				continue;

			int lo = max(0, L - lcp);
			int hi = min({lcs_len, L - 1, q});
			if (lo > hi)
				continue;

			// s = q - t, t in [lo, hi]
			int start_l = q - hi;
			int start_r = q - lo;
			int end_l = start_l + 2*L - 1;
			int end_r = start_r + 2*L - 1;

			sd[start_l]++;
			sd[start_r + 1]--;
			pd[end_l]++;
			pd[end_r + 1]--;
		}
	}

	i64 ans = 0;
	for (int i = 0; i < n; i++)
	{
		if (i)
		{
			pd[i] += pd[i-1];
			sd[i] += sd[i-1];
		}
		if (i && i < n-1) // split between i-1 and i, where AA ends at i-1 and BB starts at i
			ans += pd[i-1] * sd[i];
	}

	return ans;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin >> T;
	while (T--)
	{
		cin >> s;
		cout << solve() << '\n';
	}
	return 0;
}
```
