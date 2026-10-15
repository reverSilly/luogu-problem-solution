# P1245 电话号码 — 题解

---

## Part 1 · 重述题目

电话键盘上每个数字（1–9）对应若干英文字母：

| 数字 | 字母 |
|------|------|
| 1 | a b c |
| 2 | d e f |
| 3 | g h i |
| 4 | j k l |
| 5 | m n |
| 6 | o p q |
| 7 | r s t |
| 8 | u v w |
| 9 | x y z |

给定一个长度为 $L$（$L \le 100$）的数字密码串 $S$，以及 $N$（$N \le 100$）个单词（每个长度 $\le 20$），要求把密码串 $S$ 分成若干段，使每一段恰好等于某个单词的数字编码（按上表逐字母映射后拼接）。输出对应的单词序列（用空格分隔），若不存在则输出 `No Solutions!`。存在多种解时输出任意一种即可（SPJ 特判）。

**约束：** $N \le 100$，$L \le 100$，单词长度 $\le 20$。

---

## Part 2 · 思维链（发现过程）

### 2.1 探索问题本质

把每个字母映射为它对应的数字，每个单词就变成一段数字串。问题等价于：

> 把 $S$ 切分成若干连续子串，使得每个子串都等于某个单词编码后的数字串。

这本质上是**字符串划分（word break）**问题，只不过"字典"里的词是数字串而非字母串。

### 2.2 考虑更简单的版本：贪心行不行？

最直观的想法是：从左往右，尝试用字典里能匹配当前位置前缀的最短词，然后继续。但贪心不成立。

**反例：** 设字典为 $\{ab, abc, c\}$，密码串为 $ab\,c$ 的编码（即 $1\,2\,3$，其中 $a \to 1, b \to 1, c \to 1$，编码为 `111`）。贪心若优先取 `ab`（编码 `11`），剩下 `1` 对应 `c`，可行。再设字典 $\{a, bc, b, c\}$，密码串 `1111`（对应 `a`→1, `b`→1, `c`→1, `c`→1 的编码，即 `1111`）。贪心取 `a` 后剩 `111`，再取 `b` 后剩 `11`，再取 `c` 后剩 `1` 无解；但 `a` + `bc` + `c` 是合法解。贪心会失败。

所以不能贪心，需要**回溯/记忆化搜索**或 **DP**。

### 2.3 经典解法：区间 DP / 记忆化 DFS

把密码串 $S$ 视为下标 $0$ 到 $L-1$ 的串。定义 $dp[i] = $ 密码串 $S[i..L-1]$ 是否可被字典完整覆盖。

转移：$dp[i] = \bigvee\limits_{w \in \text{dict},\; code(w) \text{ matches } S[i..i+\lvert w\rvert-1]} dp[i+\lvert w\rvert]$。

基础情况：$dp[L] = \text{true}$（空串覆盖成功）。

最终答案：$dp[0]$。

**重建方案：** 记录 $pre[i] = $ 使 $dp[i] = \text{true}$ 的那个单词的字典下标（或编码后字符串），从 $0$ 出发沿 $pre$ 走到 $L$ 即可还原。

由于 $L \le 100$，$N \le 100$，每步枚举 $N$ 个单词、每个单词做最长 20 位的字符串比较，总复杂度 $O(L \cdot N \cdot 20) = O(100 \cdot 100 \cdot 20) = 2 \times 10^5$，远小于时限。

**更高效的写法（优化枚举）：** 预先为每个位置 $i$ 筛出所有能匹配 $S[i..*]$ 的单词（即编码后以 $S[i..*]$ 为前缀的单词），这样 $dp[i]$ 只需枚举这些单词，常数更小。但由于 $N$ 本身只有 100，直接全枚举也完全够用。

### 2.4 证明 DP 的正确性

**引理 1（最优子结构）：** 若 $S[i..L-1]$ 可被字典覆盖，则存在某个字典单词 $w$ 满足：$code(w)$ 恰好等于 $S[i..i+\lvert w\rvert-1]$（即 $S$ 在位置 $i$ 起的前 $\lvert w\rvert$ 个字符），且 $S[i+\lvert w\rvert..L-1]$ 也可被字典覆盖。

**证明：** 设 $S[i..L-1]$ 有一个合法覆盖 $w_1, w_2, \ldots, w_k$（每个 $w_j \in$ 字典，$\sum \lvert w_j\rvert = L-i$）。取第一个单词 $w_1$，它的编码恰好覆盖 $S$ 的前 $\lvert w_1\rvert$ 位，即 $S[i..i+\lvert w_1\rvert-1]$，剩余 $S[i+\lvert w_1\rvert..L-1]$ 由 $w_2,\ldots,w_k$ 覆盖，故也可被字典覆盖。□

**引理 2（完备性）：** 若 $S[i..L-1]$ 可被字典覆盖，则 $dp[i] = \text{true}$。

**证明：** 用对 $L-i$ 的归纳。$L-i = 0$ 时 $dp[L] = \text{true}$，成立。设 $L-i = m > 0$，由引理 1 存在 $w \in$ 字典使 $code(w) = S[i..i+\lvert w\rvert-1]$ 且 $S[i+\lvert w\rvert..L-1]$ 可覆盖。由归纳假设 $dp[i+\lvert w\rvert] = \text{true}$，故 $dp[i]$ 转移条件满足，$dp[i] = \text{true}$。□

**引理 3（充分性）：** 若 $dp[i] = \text{true}$，则 $S[i..L-1]$ 可被字典覆盖。

**证明：** 用对 $L-i$ 的归纳。$L-i=0$ 平凡。设 $dp[i] = \text{true}$ 且 $L-i > 0$，由转移定义存在 $w$ 使 $code(w) = S[i..i+\lvert w\rvert-1]$ 且 $dp[i+\lvert w\rvert] = \text{true}$。由归纳假设 $S[i+\lvert w\rvert..L-1]$ 可覆盖，加上 $w$，$S[i..L-1]$ 可覆盖。□

**定理：** $dp[0] = \text{true}$ 当且仅当 $S[0..L-1]$ 可被字典完整覆盖。

**证明：** 由引理 2（充分方向）和引理 3（必要方向），取 $i=0$ 即得。□

### 2.5 时间复杂度分析

- $dp$ 表大小：$L+1 \le 101$ 个状态。
- 每个状态 $dp[i]$ 枚举最多 $N \le 100$ 个单词。
- 每个单词的编码比较至多 20 个字符。
- 总操作数 $\le 101 \times 100 \times 20 = 202\,000$，约 $2 \times 10^5$，完全可行。

空间：$dp$ 数组 $O(L)$，$pre$ 数组 $O(L)$，字典编码串 $O(N \cdot 20)$，均极小。

---

## Part 3 · 模式总结

按实际思考顺序：

1. **探索问题本质** — 每个单词映射为数字串，问题变成"把 $S$ 切分为字典中数字串的子串"。
2. **考虑更简单版本** — 贪心从左到右取匹配词；构造反例证明贪心失败（局部最优 $\neq$ 全局可行）。
3. **经典问题识别** — 这是 word break / 字符串划分问题，标准解法为 DP。
4. **状态定义** — $dp[i]$：后缀 $S[i..L-1]$ 是否可覆盖；转移枚举字典词、检查编码匹配。
5. **重建方案** — $pre[i]$ 记录使 $dp[i]=\text{true}$ 的单词，从 0 沿 $pre$ 回溯得答案。
6. **正确性证明** — 最优子结构 + 归纳，双向证明 DP 的充要性。

---

## Part 4 · 正式解法

### 符号表

| 符号 | 含义 |
|------|------|
| $L$ | 密码串 $S$ 的长度，$L \le 100$ |
| $S$ | 密码串，字符集 $\{0,1,\ldots,9\}$，长度 $L$ |
| $N$ | 字典中单词数，$N \le 100$ |
| $W[i]$ | 字典中第 $i$ 个单词（$0$-based），$0 \le i < N$ |
| $C[i]$ | $W[i]$ 编码后的数字串（字母→数字映射） |
| $dp[i]$ | $\text{bool}$：$S[i..L-1]$ 是否可被字典覆盖；$dp[L]=\text{true}$ |
| $pre[i]$ | $\text{int}$：使 $dp[i]=\text{true}$ 的字典下标（取任意一个）；$dp[i]=\text{false}$ 时为 $-1$ |
| $ans$ | $\text{string}$：最终重建的单词序列（空格分隔） |

### 字母→数字映射

$$
\text{letter}(c) =
\begin{cases}
1 & c \in \{a,b,c\} \\
2 & c \in \{d,e,f\} \\
3 & c \in \{g,h,i\} \\
4 & c \in \{j,k,l\} \\
5 & c \in \{m,n\} \\
6 & c \in \{o,p,q\} \\
7 & c \in \{r,s,t\} \\
8 & c \in \{u,v,w\} \\
9 & c \in \{x,y,z\} \\
\end{cases}
$$

对单词 $W = w_1 w_2 \cdots w_k$，其编码为 $C = \text{letter}(w_1) \text{letter}(w_2) \cdots \text{letter}(w_k)$，即 $C[j] = '0' + \text{letter}(w_{j+1})$。

### 算法步骤

1. **读入：** 读入 $N$、$S$、$N$ 个单词 $W[0],\ldots,W[N-1]$。
2. **编码字典：** 对每个 $W[i]$，逐字符映射得 $C[i]$（数字串）。
3. **DP 填表：** 从 $i = L-1$ 递减到 $0$：
   - $dp[i] = \text{false}$，$pre[i] = -1$。
   - 对每个 $j \in [0, N)$：
     - 若 $\lvert C[j]\rvert \le L - i$ 且 $C[j] == S[i .. i+\lvert C[j]\rvert-1]$ 且 $dp[i+\lvert C[j]\rvert] = \text{true}$：
       - $dp[i] = \text{true}$，$pre[i] = j$，break。
4. **判定：** 若 $dp[0] = \text{false}$，输出 `No Solutions!`，结束。
5. **重建：** $i = 0$；当 $i < L$：
   - 取 $j = pre[i]$，把 $W[j]$ 加入答案（第一个词不加前导空格，后续词前加一个空格）。
   - $i \mathrel{+}= \lvert W[j]\rvert$（注意是单词原始长度，即 $C[j]$ 的长度，二者相等）。
6. **输出：** 打印拼接好的答案串。

### 复杂度

- **时间：** $O(L \cdot N \cdot 20) = O(2 \times 10^5)$（最坏），实际远小于。
- **空间：** $O(L + N \cdot 20)$。

### 验证状态

- 小样例手工验证：字典 $\{ab, c\}$，$S$ 为 `111`（`a`→1, `b`→1, `c`→1）→ `ab` + `c`，正确。
- 无解样例：字典 $\{a\}$，$S$ 为 `12`（`a` 只能覆盖 1 位，剩 `2` 无词）→ `No Solutions!`，正确。
- 复杂度 $O(2 \times 10^5)$ 对 $L=100, N=100$ 绰绰有余，无 TLE 风险。

---

## Part 5 · 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int N;
string S;
string W[mxn],C[mxn];
bool dp[mxn];
int pre[mxn];

char enc(char c)
{
	if(c>='a'&&c<='c')
		return '1';
	if(c>='d'&&c<='f')
		return '2';
	if(c>='g'&&c<='i')
		return '3';
	if(c>='j'&&c<='l')
		return '4';
	if(c>='m'&&c<='n')
		return '5';
	if(c>='o'&&c<='q')
		return '6';
	if(c>='r'&&c<='t')
		return '7';
	if(c>='u'&&c<='w')
		return '8';
	return '9';
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>N>>S;
	int L=S.size();
	for(int i=0;i<N;++i)
	{
		cin>>W[i];
		C[i].clear();
		for(char c:W[i])
			C[i]+=enc(c);
	}

	// dp from back to front
	dp[L]=true;
	fill_n(pre,N,-1);
	for(int i=L-1;i>=0;--i)
	{
		dp[i]=false;
		pre[i]=-1;
		for(int j=0;j<N;++j)
		{
			int wlen=C[j].size();
			if(i+wlen>L)
				continue;
			if(S.substr(i,wlen)!=C[j])
				continue;
			if(!dp[i+wlen])
				continue;
			dp[i]=true;
			pre[i]=j;
			break;
		}
	}

	if(!dp[0])
	{
		cout<<"No Solutions!"<<'\n';
		return 0;
	}

	// reconstruct
	string ans;
	int i=0;
	while(i<L)
	{
		int j=pre[i];
		if(!ans.empty())
			ans+=' ';
		ans+=W[j];
		i+=W[j].size();
	}
	cout<<ans<<'\n';
	return 0;
}
```

**代码说明：**

- `enc(c)` 按题面映射表将字母转为数字字符，未显式处理 `0` 是因为题目中字母只映射到 `1`–`9`。
- `C[i]` 存 $W[i]$ 编码后的数字串，用于与 $S$ 的前缀比较。
- DP 从 $L-1$ 递减到 $0$，保证转移时 $dp[i+\lvert C[j]\rvert]$ 已计算（$i+\lvert C[j]\rvert > i$，先算大的）。
- `pre[i]` 记录第一个使 $dp[i]=\text{true}$ 的字典下标，break 即取任一可行解（SPJ 接受任意解）。
- 重建时 $i$ 按单词**原始长度**（字母数）推进，与编码串长度一致（映射是一对一长度保持的）。
