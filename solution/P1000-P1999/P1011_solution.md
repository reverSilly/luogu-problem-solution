# P1011 [NOIP 1998 提高组] 车站 题解

---

## 一、发现叙事

> **题面速览**  
> 火车从第 1 站出发，上车 \(a\) 人。第 2 站上车 = 下车 = 未知数 \(k\)，故开出时人数仍是 \(a\)。  
> 从第 3 站到第 \(n-1\) 站：上车人数 = 前两站上车人数之和，下车人数 = 上一站上车人数。  
> 终点站（第 \(n\) 站）全部 \(m\) 人下车。  
> 问：第 \(x\) 站开出时车上有多少人？

这是 NOIP 1998 提高组的第一题，标签是"数学 / Fibonacci 数列"。

初次读题，容易陷入的误区是试图直接模拟每一站的上车、下车人数——但第二站的上车人数 \(k\) 是未知的，我们只有最后的总下车人数 \(m\) 这一条约束。似乎需要先解出 \(k\)，再回答 \(x\)。

问题本身的结构非常清晰：

- **上车人数序列**：第 1 站 = \(a\)，第 2 站 = \(k\)，从第 3 站起  
  \[
  \text{on}[i] = \text{on}[i-1] + \text{on}[i-2]
  \]
  这是一个**斐波那契型递推**。

- **下车人数序列**：第 1 站 = 0，第 2 站 = \(k\)，第 \(i\ge 3\) 站  
  \[
  \text{off}[i] = \text{on}[i-1]
  \]

- **车上人数（开出时）**：  
  \[
  \text{total}[i] = \text{total}[i-1] + \text{on}[i] - \text{off}[i]
  \]

每一步都是线性的，所以 \(\text{on}[i]\) 和 \(\text{total}[i]\) 都可以写成 \(a\) 和 \(k\) 的线性组合：

\[
\text{on}[i] = A_i \cdot a + B_i \cdot k \qquad
\text{total}[i] = C_i \cdot a + D_i \cdot k
\]

其中 \(A_i, B_i, C_i, D_i\) 是与 \(a, k\) 无关的整数系数，只由递推关系决定。  
这样一来，**未知数 \(k\) 可以通过终点条件 \(\text{total}[n-1] = m\) 解出**，再代回得到答案。

整道题本质就是**系数追踪**——一个小规模的斐波那契型线性递推系统。

---

## 二、思维链条与完整证明

### 2.1 定义符号

| 符号 | 含义 |
|------|------|
| \(a\) | 始发站上车人数（已知） |
| \(k\) | 第 2 站上车人数（未知） |
| \(n\) | 车站总数 |
| \(m\) | 终点站下车人数（即全程总人数，已知） |
| \(x\) | 所求的车站编号 |
| \(\text{on}[i]\) | 第 \(i\) 站上车人数 |
| \(\text{off}[i]\) | 第 \(i\) 站下车人数 |
| \(\text{total}[i]\) | 第 \(i\) 站开出时车上人数 |

其中 \(i\) 从 1 到 \(n-1\)（终点站 \(n\) 只下车，不在此列）。

### 2.2 题目条件的形式化

**第 1 站（始发站）：**

\[
\text{on}[1] = a, \quad \text{off}[1] = 0, \quad \text{total}[1] = a
\]

**第 2 站：**

\[
\text{on}[2] = \text{off}[2] = k
\]
\[
\text{total}[2] = \text{total}[1] + \text{on}[2] - \text{off}[2] = a + k - k = a
\]

**第 \(i\) 站（\(3 \le i \le n-1\)）：**

\[
\text{on}[i] = \text{on}[i-1] + \text{on}[i-2] \tag{1}
\]
\[
\text{off}[i] = \text{on}[i-1] \tag{2}
\]
\[
\text{total}[i] = \text{total}[i-1] + \text{on}[i] - \text{off}[i] \tag{3}
\]

**终点站（第 \(n\) 站）：**

\[
\text{total}[n-1] = m \tag{4}
\]

（终点站全部 \(m\) 人下车，即到达终点站前车上恰好有 \(m\) 人。）

### 2.3 简化 total 的递推

将 (1)(2) 代入 (3)：

\[
\begin{aligned}
\text{total}[i] &= \text{total}[i-1] + (\text{on}[i-1] + \text{on}[i-2]) - \text{on}[i-1] \\
&= \text{total}[i-1] + \text{on}[i-2] \tag{5}
\end{aligned}
\]

这是一个非常简洁的形式：**第 \(i\) 站比第 \(i-1\) 站多出的人数恰好等于第 \(i-2\) 站的上车人数**。

### 2.4 系数追踪

由于所有递推都是线性的，我们把 \(\text{on}[i]\) 和 \(\text{total}[i]\) 写成 \(a\) 和 \(k\) 的线性组合。

设：

\[
\text{on}[i] = \text{on}_a[i] \cdot a + \text{on}_k[i] \cdot k
\]
\[
\text{total}[i] = \text{total}_a[i] \cdot a + \text{total}_k[i] \cdot k
\]

其中 \(\text{on}_a[i], \text{on}_k[i], \text{total}_a[i], \text{total}_k[i]\) 都是整数系数。

**初始化：**

\[
\begin{cases}
\text{on}_a[1] = 1, & \text{on}_k[1] = 0 \\
\text{on}_a[2] = 0, & \text{on}_k[2] = 1
\end{cases}
\quad
\begin{cases}
\text{total}_a[1] = 1, & \text{total}_k[1] = 0 \\
\text{total}_a[2] = 1, & \text{total}_k[2] = 0
\end{cases}
\]

**递推（对 \(i \ge 3\)）：**

由 (1) 得：

\[
\begin{aligned}
\text{on}_a[i] &= \text{on}_a[i-1] + \text{on}_a[i-2] \tag{6} \\
\text{on}_k[i] &= \text{on}_k[i-1] + \text{on}_k[i-2] \tag{7}
\end{aligned}
\]

由 (5) 得：

\[
\begin{aligned}
\text{total}_a[i] &= \text{total}_a[i-1] + \text{on}_a[i-2] \tag{8} \\
\text{total}_k[i] &= \text{total}_k[i-1] + \text{on}_k[i-2] \tag{9}
\end{aligned}
\]

### 2.5 系数封闭形式（斐波那契数列）

回忆斐波那契数列的定义：

\[
F_0 = 0,\quad F_1 = 1,\quad F_i = F_{i-1} + F_{i-2}\;(i \ge 2)
\]

对比 (6)(7) 与斐波那契递推，可以写出封闭形式（对 \(i \ge 3\)）：

\[
\begin{aligned}
\text{on}_a[i] &= F_{i-2} \quad (\text{因为 on}_a[1]=1, \text{on}_a[2]=0) \\
\text{on}_k[i] &= F_{i-1} \quad (\text{因为 on}_k[1]=0, \text{on}_k[2]=1)
\end{aligned}
\]

验证：

| \(i\) | \(\text{on}_a[i]\) | \(F_{i-2}\) | \(\text{on}_k[i]\) | \(F_{i-1}\) |
|------|--|--|--|--|
| 3 | 1 | \(F_1=1\) | 1 | \(F_2=1\) |
| 4 | 1 | \(F_2=1\) | 2 | \(F_3=2\) |
| 5 | 2 | \(F_3=2\) | 3 | \(F_4=3\) |
| 6 | 3 | \(F_4=3\) | 5 | \(F_5=5\) |

而 \(\text{total}_a[i]\) 和 \(\text{total}_k[i]\) 则由 (8)(9) 累加得到。由于 \(n \le 20\)，我们完全可以通过循环直接计算这些系数，不需要显式的封闭形式。

### 2.6 解出未知数 \(k\)

由条件 (4)：

\[
\text{total}[n-1] = \text{total}_a[n-1] \cdot a + \text{total}_k[n-1] \cdot k = m
\]

移项得：

\[
k = \frac{m - \text{total}_a[n-1] \cdot a}{\text{total}_k[n-1]} \tag{10}
\]

题设保证解出的 \(k\) 是整数。

### 2.7 回答所求

\[
\text{answer} = \text{total}[x] = \text{total}_a[x] \cdot a + \text{total}_k[x] \cdot k
\]

若 \(x = n\)，则终点站开出时车上已无人，答案为 \(0\)。

---

## 三、模式总结

> **斐波那契型系数追踪法**  
> 当系统的状态由**线性递推**刻画且含**一个未知参数**时，将每个状态用**已知量**和**未知参数**的系数对表示，再通过**边界条件**解出未知参数，最后代回求得目标状态。

本题的要点：

1. **上车人数是斐波那契递推** — \(\text{on}[i] = \text{on}[i-1] + \text{on}[i-2]\)  
2. **下车人数等于上一站上车** — \(\text{off}[i] = \text{on}[i-1]\)  
3. **总人数递推简化** — \(\text{total}[i] = \text{total}[i-1] + \text{on}[i-2]\)  
4. **系数追踪** — 用 \((\text{coeff}_a, \text{coeff}_k)\) 表示每个量，避开未知数 \(k\) 直接模拟  
5. **利用边界 \(m\) 反解 \(k\)** — 再用 \(k\) 求出第 \(x\) 站的值

---

## 四、真实解法与符号表

### 符号表

| 变量 | 含义 |
|------|------|
| `a,n,m,x` | 题目输入 |
| `on_a[i], on_k[i]` | \(\text{on}[i]\) 中 \(a\) 和 \(k\) 的系数 |
| `total_a[i], total_k[i]` | \(\text{total}[i]\) 中 \(a\) 和 \(k\) 的系数 |

### 算法步骤

```
1. 读入 a, n, m, x

2. 初始化系数（1-indexed）：
   on_a[1] = 1, on_k[1] = 0
   on_a[2] = 0, on_k[2] = 1
   total_a[1] = 1, total_k[1] = 0
   total_a[2] = 1, total_k[2] = 0

3. 对 i = 3 到 n-1：
   on_a[i] = on_a[i-1] + on_a[i-2]
   on_k[i] = on_k[i-1] + on_k[i-2]
   total_a[i] = total_a[i-1] + on_a[i-2]
   total_k[i] = total_k[i-1] + on_k[i-2]

4. k = (m - total_a[n-1] * a) / total_k[n-1]

5. 如果 x < n：
     答案 = total_a[x] * a + total_k[x] * k
   否则：
     答案 = 0

6. 输出答案
```

**复杂度**：\(O(n)\) 时间，\(O(1)\) 额外空间（\(n \le 20\)）。  
**注**：由于约束极小，完全不需任何优化，按定义模拟即可。

---

## 五、C++ 代码

```cpp
#include<bits/stdc++.h>
using namespace std;

int a,n,m,x;
int on_a[25],on_k[25];
int total_a[25],total_k[25];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>a>>n>>m>>x;

	// 第 1 站
	on_a[1]=1;on_k[1]=0;
	total_a[1]=1;total_k[1]=0;

	// 第 2 站
	on_a[2]=0;on_k[2]=1;
	total_a[2]=1;total_k[2]=0;

	// 第 3 ~ n-1 站
	for(int i=3;i<n;++i)
	{
		// on[i]=on[i-1]+on[i-2]
		on_a[i]=on_a[i-1]+on_a[i-2];
		on_k[i]=on_k[i-1]+on_k[i-2];
		// total[i]=total[i-1]+on[i-2]
		total_a[i]=total_a[i-1]+on_a[i-2];
		total_k[i]=total_k[i-1]+on_k[i-2];
	}

	// total[n-1]=m  =>  k=(m-total_a[n-1]*a)/total_k[n-1]
	int k=(m-total_a[n-1]*a)/total_k[n-1];

	int ans;
	if(x<n)
		ans=total_a[x]*a+total_k[x]*k;
	else
		ans=0;

	cout<<ans<<'\n';

	return 0;
}
```

---

> **附注**：若题目不保证 \(x=n\) 的情况（约束仅保证 \(x\le n\)），当 \(x=n\) 时输出 0 是符合题意的（终点站开出后车上无人）。但如果题目期望输出 \(m\)（即到达终点站时的人数），可将 \(x=n\) 分支改为 `ans=m`。两种理解都可以通过示例，请以实际评测为准。
