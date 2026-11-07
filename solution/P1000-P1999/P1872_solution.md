# P1872 回文串计数

## 1. 重新读题

### 1.1 问题重述（人话版）

给定一个长度不超过 $2000$ 的小写字符串 $SS$。数四元组 $(l,r,L,R)$ 的个数，满足

$$1\le l\le r<L\le R\le \mathrm{Len},$$

并且子串 $S[l,r]$ 与 $S[L,R]$ 都是回文串。

也就是说：在串里找**前后不重叠**的两个回文子串（第一个必须在第二个左边，中间可以被其他字符隔开），数这样的有序二元组个数。

### 1.2 关键约束与规模暗示

- $\mathrm{Len}\le2000$。子串总数 $\frac{n(n+1)}2\approx2\times10^6$，所以"把所有子串是否回文判出来"是 $O(n^2)$ 可行的（$4\times10^6$ 个区间状态）。
- 但四元组有四个自由度，直接枚举是 $O(n^4)$（$1.6\times10^{13}$），必须找结构把其中两个自由度合并成一个计数。
- 输出是整数，没有取模：最大值可达 $\sim n^4/24\approx6.7\times10^{11}$，超过 `int`，必须 `long long`。

## 2. 思维链（Chain-of-thought）

### 2.1 第一步：先把"回文"这件事变成可查询的

要在 $O(n^2)$ 里判定所有子串，最自然的想法是区间 DP。设 `pal[l][r]` 表示 $S[l,r]$ 是否回文（$0$ 起）。长度 $1$ 一定是回文；长度 $2$ 看两字符是否相等；更长时：

$$\texttt{pal}[l][r]=\bigl(S_l=S_r\bigr)\ \wedge\ \texttt{pal}[l+1][r-1].$$

**引理 1（回文的区间递推）**：上式与回文定义等价。

**证明**：

- （$\Leftarrow$）若 $S_l=S_r$ 且 $S[l+1,r-1]$ 回文，则 $S[l,r]$ 从两端往中间读完全对称，是回文；长度为 $2$ 时中间为空，只需首尾相等，也对。
- （$\Rightarrow$）若 $S[l,r]$ 回文，则首尾字符必相等；去掉首尾后的 $S[l+1,r-1]$ 反写等于自身（回文串的子串去首尾仍是回文），所以 $\texttt{pal}[l+1][r-1]$ 为真。

对长度归纳即得。$\square$

按长度从小到大计算，复杂度 $O(n^2)$，空间 $O(n^2)$（$2000^2$ 个 `bool`，约 $4$ MB）。

### 2.2 第二步：数四元组时，谁才是"枢纽"

暴力枚举 $l,r,L,R$ 时，约束 $r<L$ 是唯一把左右两段联系起来的东西。观察：一旦固定了 $L$，左右两段就完全独立了：

- 左段只需要满足 $r<L$，即整个左段落在前缀 $[1,L-1]$ 内；
- 右段只需要满足左端点为 $L$，右端点 $R\ge L$ 任意。

于是固定 $L$ 后，组合数是"左段可选个数 × 右段可选个数"，横跨两段的约束只有 $r<L$，已经被 $L$ 吸收。这就是把四层枚举拆成两个独立计数的地方。

### 2.3 第三步：定义两个计数数组

- `cntS[L]`：以 $L$ 为**左端点**的回文子串个数，即满足 $\texttt{pal}[L][R]$ 的 $R$ 的个数。
- `pre[L]`：右端点 $<L$ 的回文子串个数（左段必须在 $[0,L-1]$ 内），即
  $$\texttt{pre}[L]=\sum_{r<L}\texttt{cntE}[r],\qquad \texttt{cntE}[r]=\#\{l:\texttt{pal}[l][r]\}.$$

**引理 2（答案的乘积分解）**：
$$\text{答案}=\sum_{L=0}^{n-1}\texttt{pre}[L]\cdot\texttt{cntS}[L].$$

**证明**：

建立映射：每个合法四元组 $(l,r,L,R)\mapsto L$。

- 对固定的 $L$，前半段的可能形态是"右端点 $<L$ 的回文子串"，数量正是 `pre[L]`；后半段的可能形态是"以 $L$ 为左端点的回文子串"，数量正是 `cntS[L]`。两者可以任意组合：左段的 $r\le L-1<L\le R$，所以组合出来的四元组一定满足 $r<L$ 且两段都是回文。不同组合给出不同四元组（$(l,r)$ 或 $R$ 不同），故固定 $L$ 的合法四元组数 $=\texttt{pre}[L]\cdot\texttt{cntS}[L]$。
- 不同的 $L$ 对应的四元组集合不交（$L$ 是四元组的一部分）。所以总和就是全部合法四元组数。$\square$

### 2.4 第四步：统计数组

先由 `pal` 一次 $O(n^2)$ 遍历：

- 对每个 $l$，数有多少 $r\ge l$ 使 `pal[l][r]`，得 `cntS[l]`；
- 对每个 $r$，数有多少 $l\le r$ 使 `pal[l][r]`，得 `cntE[r]`。

再对 `cntE` 做前缀和得 `pre`（`pre[L]=pre[L-1]+cntE[L-1]`，`pre[0]=0`）。

答案 $\sum_{L=0}^{n-1}\texttt{pre}[L]\cdot\texttt{cntS}[L]$，用 `long long` 存。

### 2.5 边界与验证

- 长度为 $1$ 的子串都是回文，所以 `cntS`、`cntE` 至少各含自己那个单字符。
- 样例 $SS=\texttt{aaa}$：回文子串为 $[0,0],[1,1],[2,2],[0,1],[1,2],[0,2]$。`cntE=[1,2,3]`，`cntS=[3,2,1]`，`pre=[0,1,3]`。答案 $=0\cdot3+1\cdot2+3\cdot1=5$，与题面给出的 $5$ 对一致。
- 答案上界估计：回文子串最多 $\frac{n(n+1)}2\approx2\times10^6$ 个，两两配对的上界是它的平方（$4\times10^{12}$）——`int` 必然溢出。更精确地，合法四元组要求 $r<L$，可以把每对子串按 $L$ 分开计数，量级 $\frac{n^4}{24}\approx6.7\times10^{11}$，仍然超 `int`。所以必须 64 位。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 四元组的两段只通过 $r<L$ 耦合，固定 $L$ 后左右独立。
- **classic problem solution** — 区间 DP 在 $O(n^2)$ 内判定所有子串是否回文。
- **reduce to easier problem** — 把四层枚举降成"固定分界 $L$"的一层求和，答案写成前缀计数乘后缀计数。
- **prefix sum** — 用前缀和把"右端点 $<L$ 的回文子串数"从 $O(n)$ 查询降到 $O(1)$。

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| $s$ | `string` | 输入字符串 | 读入 | — |
| $n$ | `int` | $s$ 的长度 | `s.size()` | — |
| `pal[l][r]` | `bool [2005][2005]` | 子串 $[l,r]$ 是否回文 | 长度 $1$ 为真 | palindrome |
| `cnts[l]` | `int [2005]` | 以 $l$ 为左端点的回文子串数 | $0$ | start |
| `cnte[r]` | `int [2005]` | 以 $r$ 为右端点的回文子串数 | $0$ | end |
| `pre[L]` | `i64 [2005]` | 右端点 $<L$ 的回文子串数 | `pre[0]=0` | prefix |
| `ans` | `i64` | 答案 | $0$ | — |

### 4.2 算法步骤

1. 读入字符串，令 $n=|s|$。
2. 回文 DP：对所有 $i$ 令 `pal[i][i]=true`；对 $len=2..n$ 与所有 $l$，令 $r=l+len-1$，若 $s_l=s_r$ 且（$len=2$ 或 `pal[l+1][r-1]`），则 `pal[l][r]=true`。
3. 一次 $O(n^2)$ 扫描所有 $l\le r$：若 `pal[l][r]`，则 `++cnte[r]`、`++cnts[l]`。
4. 前缀和：`pre[i+1]=pre[i]+cnte[i]`（$0\le i<n$）。
5. 累加 `ans += pre[L]*cnts[L]`（$0\le L<n$），输出 `ans`。

### 4.3 复杂度

- 时间 $O(n^2)$：回文 DP 与统计各 $O(n^2)$，求和 $O(n)$。
- 空间 $O(n^2)$：`bool` 表约 $4$ MB（$2000\times2000$）。

## 5. 示例代码

```cpp
//
// 题目：P1872 回文串计数
// https://www.luogu.com.cn/problem/P1872
// 自测：样例 aaa -> 5；小数据暴力对拍通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2005};

string s;
int n;
bool pal[mxn][mxn];
int cnte[mxn],cnts[mxn];
i64 pre[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>s;
	n=s.size();
	for(int i=0;i<n;++i)
		pal[i][i]=true;
	for(int len=2;len<=n;++len)
		for(int l=0;l+len<=n;++l)
		{
			int r=l+len-1;
			if(s[l]==s[r]&&(len==2||pal[l+1][r-1]))
				pal[l][r]=true;
		}
	for(int l=0;l<n;++l)
		for(int r=l;r<n;++r)
			if(pal[l][r])
			{
				++cnte[r];          // 以 r 结尾
				++cnts[l];          // 以 l 开头
			}
	for(int i=0;i<n;++i)
		pre[i+1]=pre[i]+cnte[i];    // pre[L] = 右端点 < L 的回文子串数
	i64 ans=0;
	for(int L=0;L<n;++L)
		ans+=pre[L]*cnts[L];        // 以 L 分界，左段在前、右段从 L 开始
	cout<<ans<<'\n';
	return 0;
}
```
