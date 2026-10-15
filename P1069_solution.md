# P1069 [NOIP 2009 普及组] 细胞分裂 — 题解

## 一、发现叙事

拿到题目，先读清楚场景：有 $N$ 种细胞，第 $i$ 种每秒分裂为 $S_i$ 个（乘性增长）。我们要选一种细胞，从 1 个开始培养，经过 $t$ 秒后得到 $S_i^{\,t}$ 个细胞，要求这个数能被 $M = m_1^{m_2}$ 整除。目标是**最小化 $t$**，如果所有细胞种类都不可能，输出 $-1$。

### 第一眼：暴力思路

最朴素的想法是枚举 $t = 0,1,2,\dots$，每次计算 $S_i^{\,t} \bmod M$，直到整除或发现不可能。但 $M$ 可能大到 $(30000)^{10000}$，根本没法直接算。

50% 的数据说 $m_1^{m_2} \le 30000$，暴力或许可行——但剩下 50% 的数据 $m_2$ 可达 10000，$M$ 巨大无比，暴力不可行。

### 转向数论视角

"整除"即 $M \mid S_i^{\,t}$。写开：

$$
M = m_1^{m_2} = \bigl(\prod p_j^{a_j}\bigr)^{m_2} = \prod p_j^{a_j m_2}
$$

所以 $M \mid S_i^{\,t}$ 等价于：**对 $m_1$ 的每个素因子 $p_j$，$S_i^{\,t}$ 中 $p_j$ 的指数不小于 $a_j m_2$**。

设 $S_i$ 中 $p_j$ 的指数为 $b_j$（$b_j$ 可以为 0），则 $S_i^{\,t}$ 中 $p_j$ 的指数是 $t \cdot b_j$。条件为：

$$
t \cdot b_j \ge a_j m_2
$$

- 如果 $b_j = 0$ 但 $a_j > 0$：无论 $t$ 多大，左边的指数永远是 0，永远无法达到 $a_j m_2 > 0$，直接判死刑。
- 如果 $b_j > 0$：$t \ge \lceil \frac{a_j m_2}{b_j} \rceil$。

对所有素因子取最紧的约束：

$$
t_i = \max_{p_j \mid m_1} \bigl\lceil \frac{a_j m_2}{\operatorname{v}_{p_j}(S_i)} \bigr\rceil
$$

其中 $\operatorname{v}_{p_j}(S_i)$ 是 $S_i$ 中 $p_j$ 的指数。

答案就是 $\min_i t_i$，如果所有 $i$ 都不可能则输出 $-1$。

### 特例

- $m_1 = 1$：$M = 1^{m_2} = 1$，任何正整数都能被 1 整除，所以 $t = 0$ 秒即可。直接输出 $0$。
- $S_i = 1$：$1^t = 1$，除非 $M = 1$，否则永远无法满足。

---

## 二、完整推理链与证明

### 2.1 问题形式化

给定 $N$ 个正整数 $S_1,\dots,S_N$，以及 $M = m_1^{m_2}$。求最小的非负整数 $t$，使得存在某个 $i$ 满足：

$$
S_i^{\,t} \equiv 0 \pmod{M}
$$

若不存在则输出 $-1$。

### 2.2 整除与素因子分解

**引理 1.** 设整数 $a,b > 0$，$a = \prod p^{e_p}$，$b = \prod p^{f_p}$。则 $a \mid b$ 当且仅当对所有素数 $p$ 有 $e_p \le f_p$。

证明：由整数唯一分解定理直接得到。$\square$

**推论.** $M \mid S_i^{\,t}$ 当且仅当对 $m_1$ 的每个素因子 $p$，有 $\operatorname{v}_p(M) \le \operatorname{v}_p(S_i^{\,t})$。

### 2.3 指数增长下的条件

设 $m_1 = \prod p_j^{a_j}$，则 $M = \prod p_j^{a_j m_2}$，所以 $\operatorname{v}_{p_j}(M) = a_j m_2$。

设 $S_i = \prod_k q_k^{b_k}$ 是 $S_i$ 的素因子分解。对于素因子 $p_j$：

- 若 $p_j \nmid S_i$，则 $\operatorname{v}_{p_j}(S_i) = 0$，从而 $\operatorname{v}_{p_j}(S_i^{\,t}) = t \cdot 0 = 0$。
  要满足 $0 \ge a_j m_2$，必须 $a_j m_2 = 0$。由于 $a_j \ge 1$（$p_j$ 因子的指数），$m_2 \ge 1$，故不可能。
- 若 $p_j \mid S_i$，设 $\operatorname{v}_{p_j}(S_i) = b_j \ge 1$，则 $\operatorname{v}_{p_j}(S_i^{\,t}) = t \cdot b_j$。
  条件：$t \cdot b_j \ge a_j m_2 \implies t \ge \lceil a_j m_2 / b_j \rceil$。

**定理 1.** 对于细胞类型 $i$，可行的 $t$ 存在当且仅当 $m_1$ 的每个素因子都整除 $S_i$。此时最小 $t$ 为：

$$
t_i = \max_{p_j \mid m_1} \left\lceil \frac{a_j m_2}{\operatorname{v}_{p_j}(S_i)} \right\rceil
$$

证明：必要性——若存在 $p_j \mid m_1$ 但 $p_j \nmid S_i$，由上述分析，永远无法满足 $p_j$ 的指数条件。充分性——当所有 $p_j \mid S_i$ 时，取 $t = t_i$ 即满足每个素因子的条件，由引理 1 知 $M \mid S_i^{\,t}$。$t_i$ 是最小值，因为每个约束都是紧的。$\square$

### 2.4 算法正确性

**定理 2.** 最终答案为 $\displaystyle \min_{i=1}^N t_i$，其中若第 $i$ 种不可能则 $t_i = +\infty$。若所有 $t_i$ 均为 $+\infty$ 则输出 $-1$。

证明：由定理 1，$t_i$ 是第 $i$ 种细胞的最小可行时间。最优方案即取各类型中的最小值。$\square$

### 2.5 边界情况

- $m_1 = 1$：此时 $M = 1$，$M$ 没有素因子。条件 $\prod p_j^{0} \mid S_i^{\,t}$ 恒成立。任何细胞在 $t = 0$ 时 $S_i^0 = 1$ 即满足。输出 $0$。
- $S_i = 1$：$1^t = 1$，只有 $M = 1$ 时才可行。

---

## 三、模式总结

| 观察点 | 转化 |
|--------|------|
| $M$ 太大无法直接计算 | 分解素因子，逐素因子判断 |
| 整除条件 | $M \mid S^t \iff$ 每个素因子的指数条件 |
| 指数增长 | $S^t$ 中指数 $= t \times S$ 中指数 |
| 多个约束取最紧 | 每个素因子给出 $t$ 的下界，取 $\max$ |
| 多个细胞取最优 | 各细胞最小 $t$ 取 $\min$ |
| $m_1 = 1$ 特例 | 直接输出 $0$ |

**核心思想**：**素因子分解 + 指数比较**。将整除问题转化为每个素因子上的指数不等式，利用乘法转化为加法（$t$ 乘指数），将"找最小 $t$"转化为"对每个素因子算 $\lceil \frac{a \cdot m_2}{b} \rceil$ 再取 $\max$"。

---

## 四、正式解法与符号表

### 符号约定

| 符号 | 含义 |
|------|------|
| $N$ | 细胞种类数 |
| $m_1, m_2$ | 试管数 $M = m_1^{m_2}$ |
| $S_i$ | 第 $i$ 种细胞每秒分裂个数 |
| $p_j$ | $m_1$ 的第 $j$ 个素因子 |
| $a_j$ | $p_j$ 在 $m_1$ 中的指数 |
| $b_{i,j}$ | $p_j$ 在 $S_i$ 中的指数（即 $\operatorname{v}_{p_j}(S_i)$） |
| $t_i$ | 第 $i$ 种细胞的最小可行时间 |
| $ans$ | 全局最小可行时间 |

### 算法步骤

1. **特判**：如果 $m_1 = 1$，直接输出 $0$。
2. **分解 $m_1$**：对 $m_1$ 做质因数分解，得到 $(p_j, a_j)$ 对。
3. **对每种细胞 $S_i$ 处理**：
   - 初始 $t_i = 0$。
   - 对每个素因子 $p_j$：
     - 计算 $b = \operatorname{v}_{p_j}(S_i)$（即 $S_i$ 中 $p_j$ 的指数）。
     - 若 $b = 0$：此细胞不可能，标记跳过。
     - 否则计算 $\lceil (a_j \cdot m_2) / b \rceil$，更新 $t_i = \max(t_i, \text{该值})$。
   - 若该细胞可行，用 $t_i$ 更新全局最小值 $ans$。
4. 若 $ans$ 未被更新（仍为 $+\infty$），输出 $-1$，否则输出 $ans$。

### 复杂度分析

- 分解 $m_1$：$O(\sqrt{m_1}) \le O(\sqrt{30000}) \approx 173$。
- 对每个 $S_i$ 试除所有素因子：$O(N \cdot \omega(m_1))$，其中 $\omega(m_1)$ 是 $m_1$ 的相异素因子个数，不超过 $6$（因为 $2 \cdot 3 \cdot 5 \cdot 7 \cdot 11 \cdot 13 = 30030 > 30000$）。
- 总复杂度：$O(\sqrt{m_1} + N \cdot \omega(m_1))$，在 $N = 10000$ 时非常快。

---

## 五、C++ 代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int inf{1145141919};

int n,m1,m2,ans{inf};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m1>>m2;

	if(m1==1)
	{
		cout<<"0\n";
		return 0;
	}

	// 对 m1 做质因数分解
	vector<pair<int,int>>fac; // (prime, exponent)
	int tmp=m1;
	for(int i=2;i*i<=tmp;++i)
	{
		if(tmp%i==0)
		{
			int cnt=0;
			while(tmp%i==0)
			{
				tmp/=i;
				++cnt;
			}
			fac.push_back({i,cnt});
		}
	}
	if(tmp>1)
		fac.push_back({tmp,1});

	for(int k=0,s;k<n;++k)
	{
		cin>>s;
		int cur=0;
		bool ok=true;

		for(auto[p,a]:fac)
		{
			int b=0;
			while(s%p==0)
			{
				s/=p;
				++b;
			}
			if(b==0)
			{
				ok=false;
				break;
			}
			// ceil(a*m2 / b) = (a*m2 + b - 1) / b
			int need=(a*m2+b-1)/b;
			if(need>cur)
				cur=need;
		}

		if(ok&&cur<ans)
			ans=cur;
	}

	if(ans==inf)
		cout<<"-1\n";
	else
		cout<<ans<<'\n';

	return 0;
}
```

### 代码要点说明

- **`fac` 存素因子**：`pair<int,int>` 分别存素数和指数，直接在分解时构造。
- **对 $S_i$ 试除**：用每个素因子依次除 $S_i$，统计指数。这样 $S_i$ 会被逐步约小。
- **上取整技巧**：`(a*m2 + b - 1) / b` 是整数上取整的标准写法，避免了浮点数。
- **$m_1 = 1$ 特判**：此时 $M = 1$，$t = 0$ 恒成立，直接输出。
- **哨兵值**：`ans` 初始化为 `inf = 1145141919`，若未被更新说明无解。
- **Allman 花括号**：所有函数体、控制结构的左花括号都在新行。
- **无多余注释**：代码尽量自解释。
