# P1118 [USACO06FEB] Backward Digit Sums G/S 题解

---

## 1. 重读题目（Re-read the problem）

> 给定 $N$（$1\le N\le 12$）和一个目标值 $sum$（$1\le sum\le 12345$）。  
> 将 $1\sim N$ 这 $N$ 个数排成一个排列 $p_1,p_2,\dots,p_N$，然后反复执行"相邻相加"操作：
>
> 1. 第一轮：$[p_1,p_2,\dots,p_N]$  
> 2. 第二轮：$[p_1+p_2,\;p_2+p_3,\;\dots,\;p_{N-1}+p_N]$  
> 3. 第三轮：将第二轮的结果再次相邻相加……  
> 4. 直到只剩一个数。
>
> 这个最后的数等于输入的 $sum$。  
> 要求：输出**字典序最小**的排列 $p_1..p_N$，若无解则不输出任何内容。

### 观察

- $N$ 的规模极小（$\le 12$），暗示搜索/枚举是可行的。
- 但 $12! = 479\,001\,600$，纯暴力不可接受，必须有数学化简和剪枝。
- 字典序最小 → 从小到大尝试候选数字，第一次搜到就是答案。

---

## 2. 思维链与完整证明（Chain-of-thought with full proofs）

### 2.1 相邻相加的本质——杨辉三角 / 二项式系数

设 $N=4$，序列为 $a,b,c,d$：

```
   a     b     c     d
     a+b   b+c   c+d
      a+2b+c  b+2c+d
       a+3b+3c+d
```

最终结果 $= a + 3b + 3c + d$。  
系数是 $1,3,3,1$，即 $\binom{3}{0},\binom{3}{1},\binom{3}{2},\binom{3}{3}$。

对一般的 $N$，设排列为 $p_1,p_2,\dots,p_N$，反复相邻相加 $N-1$ 轮后得到：

$$
\text{final} = \sum_{i=1}^{N} \binom{N-1}{i-1} \cdot p_i
\tag{1}
$$

**证明（数学归纳法）：**

- $N=1$：只有一个数 $p_1$，系数为 $1 = \binom{0}{0}$，成立。
- 设 $N=k$ 时成立。对 $N=k+1$，第一轮后得到 $k$ 个数：
  $q_i = p_i + p_{i+1},\; i=1..k$
  由归纳假设，最终结果 $= \sum_{i=1}^{k} \binom{k-1}{i-1}\cdot q_i$

  代入 $q_i$：
  $$
  \begin{aligned}
  \text{final}
  &= \sum_{i=1}^{k} \binom{k-1}{i-1}(p_i + p_{i+1}) \\
  &= \binom{k-1}{0}p_1 + \sum_{i=2}^{k} \bigl[\binom{k-1}{i-1}+\binom{k-1}{i-2}\bigr] p_i + \binom{k-1}{k-1}p_{k+1}
  \end{aligned}
  $$

  由组合恒等式 $\binom{k-1}{i-1}+\binom{k-1}{i-2}=\binom{k}{i-1}$，且 $\binom{k-1}{0}=\binom{k}{0}=1$，$\binom{k-1}{k-1}=\binom{k}{k}=1$，得：

  $$
  \text{final} = \sum_{i=1}^{k+1} \binom{k}{i-1}\, p_i
  $$

  即 $N=k+1$ 时也成立。∎

---

### 2.2 问题转化

记系数数组 $c_i = \binom{N-1}{i-1}$（$i=1..N$），原问题等价于：

> 求字典序最小的排列 $p_1..p_N$（$p$ 是 $1..N$ 的一个排列），使得
>
> $$
> \sum_{i=1}^{N} c_i \cdot p_i = sum
> $$

例：$N=4$，$c = [1,3,3,1]$，若排列为 $3,1,2,4$，则

$$
1\cdot3 + 3\cdot1 + 3\cdot2 + 1\cdot4 = 3+3+6+4 = 16
$$

与题面示例一致。

---

### 2.3 搜索框架与剪枝原理

由于 $N\le 12$，可以用 DFS 逐位确定排列，配合**可行性剪枝**。

设当前已确定前 $k$ 位（$k=0..N$），计算部分和：

$$
\text{cur} = \sum_{i=0}^{k-1} c_i \cdot a_i
$$

接下来要在位置 $k..N-1$ 填入剩下的 $r = N-k$ 个数字，剩余系数为 $c_k, c_{k+1}, \dots, c_{N-1}$。

**核心剪枝——重排不等式（Rearrangement Inequality）：**

> 对两个序列 $x_1\le x_2\le\cdots\le x_r$ 和 $y_1\ge y_2\ge\cdots\ge y_r$（异序），
> $\sum x_i y_i$ 是所有排列配对中最小的。
>
> 若两个序列都升序（同序），则 $\sum x_i y_i$ 最大。

因此：

- 将剩余系数降序排列，剩余数字升序排列，对应相乘求和得 **最小可能贡献** $S_{\min}$。
- 将剩余系数降序排列，剩余数字降序排列，对应相乘求和得 **最大可能贡献** $S_{\max}$。

剪枝条件：

$$
\begin{cases}
\text{cur} + S_{\min} > sum &\Longrightarrow\; \text{剪枝（太小了也救不回来）}\\
\text{cur} + S_{\max} < sum &\Longrightarrow\; \text{剪枝（太大了也救不回来）}
\end{cases}
$$

**正确性**：若 $\text{cur} + S_{\min} > sum$，则无论如何安排剩余数字，最终总和都不可能降到 $sum$（因为 $S_{\min}$ 已经是能达到的最小值）。对称地，若 $\text{cur} + S_{\max} < sum$，则总和不可能升到 $sum$。

**字典序保证**：DFS 在每位上从小到大尝试 $1..N$ 中未使用的数字，第一个搜到的完整解自动字典序最小。

---

### 2.4 复杂度分析

- 最坏情况不加剪枝 $O(N!)$，不可行。
- 在重排不等式剪枝下，实际搜索状态数远小于 $N!$。  
  以 $N=12$、随机 $sum$ 为例，搜索空间通常仅几千至几万节点，轻松通过。
- 每次剪枝需要 $O(N\log N)$ 排序，$N\le 12$ 下可忽略。

---

## 3. 模式总结（Pattern summary）

| 模式 | 说明 |
|------|------|
| **杨辉三角 / 二项式系数** | 反复相邻相加的最终结果 = 各项乘以 $\binom{N-1}{i-1}$ |
| **DFS + 可行性剪枝** | 搜索排列，用重排不等式计算剩余数字的最小/最大贡献，若 $sum$ 不在 $[\text{cur}+S_{\min},\,\text{cur}+S_{\max}]$ 则剪枝 |
| **字典序极值** | 从小到大枚举候选，首个解即为答案 |
| **N 极小** | $N\le 12$ 使得 $O(N\log N)$ 的剪枝计算可接受 |

---

## 4. 真实解法（Real solution with notation glossary）

### 符号表

| 符号 | 含义 |
|------|------|
| $N$ | 排列长度，$1\le N\le 12$ |
| $sum$ | 目标总和 |
| $c[i]$ | $\binom{N-1}{i}$，$i=0..N-1$ |
| $a[i]$ | 当前构造的排列（0‑based） |
| $used[x]$ | 数字 $x$ 是否已被使用 |
| $cur$ | 当前已确定部分的总和 $\sum_{j=0}^{k-1} c[j]\cdot a[j]$ |
| $k$ | 已确定的位数，也是下一步要确定的下标 |
| $r$ | 剩余位数 $= N-k$ |
| $remCoef$ | 剩余系数 $c[k..N-1]$，降序排列 |
| $remNum$ | 剩余未用数字，升序排列 |
| $S_{\min}, S_{\max}$ | 剩余数字所能贡献的最小/最大总和 |
| $found$ | 是否已找到解 |

### 算法步骤

1. **输入** $N,sum$。
2. **预处理系数**：
   $$
   c[0] = 1,\qquad
   c[i] = c[i-1] \cdot \frac{N-i}{i} \quad (i=1..N-1)
   $$
   利用组合数递推公式 $\binom{N-1}{i} = \binom{N-1}{i-1} \cdot \frac{N-i}{i}$。
3. **DFS** $(k, cur)$：
   - 若 $found$ 为真 → 直接返回。
   - 若 $cur > sum$ → 剪枝。
   - 若 $k = N$（所有位已填）：
     - 若 $cur = sum$ → 输出 $a[0..N-1]$，置 $found=true$，返回。
   - **可行性剪枝**：
     - 提取剩余系数 $c[k..N-1]$，降序排序 → $remCoef$。
     - 收集未用数字，升序排序 → $remNum$。
     - 计算 $S_{\min} = \sum remNum[i] \cdot remCoef[i]$。
     - 计算 $S_{\max} = \sum remNum[r-1-i] \cdot remCoef[i]$（即最小配最大系数）。
     - 若 $cur + S_{\min} > sum$ 或 $cur + S_{\max} < sum$ → 剪枝。
   - 枚举 $i=1..N$：
     - 若 $used[i]$ → 跳过。
     - $used[i] \gets true,\; a[k] \gets i$。
     - DFS $(k+1,\; cur + c[k]\cdot i)$。
     - $used[i] \gets false$（回溯）。
4. 若结束后 $found$ 仍为假，不输出任何内容。

---

## 5. 示例代码（Example code）

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{17};
int n, sum;
int c[mxn];			// 二项式系数 C(n-1, i)
int a[mxn];			// 当前排列
bool used[mxn];		// 标记数字是否已用
bool found;			// 是否找到解

void dfs(int k, int cur)
{
	if(found)
		return;
	if(cur > sum)
		return;
	if(k == n)
	{
		if(cur == sum)
		{
			found = true;
			for(int i=0; i<n; ++i)
				cout << a[i] << " \n"[i==n-1];
		}
		return;
	}

	// ---- 可行性剪枝：重排不等式 ----
	int r = n - k;
	// 剩余系数（降序）
	vector<int> rc(c + k, c + n);
	sort(rc.begin(), rc.end(), greater<int>());
	// 剩余未用数字（升序）
	vector<int> rn;
	for(int i=1; i<=n; ++i)
		if(!used[i])
			rn.push_back(i);
	sort(rn.begin(), rn.end());

	int mnS = 0, mxS = 0;
	for(int i=0; i<r; ++i)
	{
		mnS += rn[i] * rc[i];			// 最小数 × 最大系数 → 最小贡献
		mxS += rn[r-1-i] * rc[i];		// 最大数 × 最大系数 → 最大贡献
	}
	if(cur + mnS > sum)
		return;
	if(cur + mxS < sum)
		return;
	// ---------------------------------

	for(int i=1; i<=n; ++i)
	{
		if(used[i])
			continue;
		used[i] = true;
		a[k] = i;
		dfs(k+1, cur + c[k] * i);
		used[i] = false;
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin >> n >> sum;

	// 递推组合数 C(n-1, 0..n-1)
	c[0] = 1;
	for(int i=1; i<n; ++i)
		c[i] = c[i-1] * (n - i) / i;

	dfs(0, 0);

	return 0;
}
```

### 关键细节

- **组合数递推**：`c[i] = c[i-1] * (n - i) / i` 利用 $\binom{n-1}{i} = \binom{n-1}{i-1} \cdot \frac{n-i}{i}$，每次除 $i$ 保证整除。
- **可行性剪枝**：每次 DFS 重新收集剩余数字并排序，因 $N\le 12$，额外开销可忽略。
- **输出格式**：`" \n"[i==n-1]` 利用 `true` 转为 $1$ 来选择空格或换行。
- **字典序**：`for(int i=1; i<=n; ++i)` 天然保证从小到大枚举，首个解即字典序最小。
- **无解处理**：不输出任何内容——程序退出时不产生输出即可。
