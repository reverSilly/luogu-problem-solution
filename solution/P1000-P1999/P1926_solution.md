# P1926 小书童——刷题大军

## 1. 重新读题

### 1.1 人话重述

小 A 有 $r$ 个单位时间，要做两件事：

- **作业**：共 $m$ 项，第 $i$ 项耗时 $t_i$、得分 $v_i$；他需要做其中若干项，使总得分 $\ge k$（及格）。
- **刷题**：共 $n$ 道喜欢的题，第 $i$ 道耗时 $c_i$；及格之后，用剩下的时间尽可能多地刷这些题。

问最多能刷几道题。

数据范围：$n\le10$，$m\le10$，$k\le50$，$r\le150$，且保证有办法及格。

### 1.2 关键约束与规模暗示

- $m\le10$：作业子集只有 $2^{10}=1024$ 种，直接枚举也能过。
- $n\le10$：刷题的选择也很小。
- 但真正决定“能否及格”的是作业的**耗时与得分**，而决定“能刷几道”的是**剩余时间**与刷题耗时。两个决策通过“剩余时间”耦合。
- 数据保证及格可行，所以一定存在作业子集使总分 $\ge k$ 且耗时 $\le r$。

---

## 2. 思维链（Chain-of-thought）

### 2.1 先拆成两个独立子问题

一次完整的方案是：

1. 选一个作业子集 $S$，满足 $\sum_{i\in S}v_i\ge k$；
2. 用掉时间 $\operatorname{cost}(S)=\sum_{i\in S}t_i$，剩下
   $$
   \text{rem}=r-\operatorname{cost}(S);
   $$
3. 在 $n$ 道题里选尽可能多道，使总耗时 $\le\text{rem}$。

所以核心是：对每个可能的“作业子集”，算出它的耗时与得分，再算剩余时间能刷几道。

### 2.2 固定剩余时间：刷题部分怎么选

**引理 1（刷题贪心）**：给定剩余时间 `rem`，把刷题耗时 $c_1,\dots,c_n$ 升序排列为 $c_{(1)}\le\cdots\le c_{(n)}$。设最大的 $q$ 使得
$$
\sum_{j=1}^{q}c_{(j)}\le\text{rem},
$$
则最多能刷 $q$ 道题。

**证明**：

- 上界：任取 $q+1$ 道题，它们的总耗时至少是 $q+1$ 个最小耗时之和 $\sum_{j=1}^{q+1}c_{(j)}>\text{rem}$（由 $q$ 的最大性），所以刷不了 $q+1$ 道。
- 可达性：前 $q$ 小的题总耗时 $\le\text{rem}$，所以确实能刷 $q$ 道。

$\blacksquare$

于是“最多刷几道”只依赖于 `rem`，可以先用 $O(n\log n)$ 排序，再用前缀和 $O(\log n)$ 或直接线性扫描求出。

### 2.3 作业部分：只关心“花最少时间拿够分”

我们并不需要保留作业的具体选择，只需要知道：**拿到至少 $s$ 分最少要花多少时间**。

定义
$$
dp[s]=\text{使用前若干项作业，得到至少 }s\text{ 分所需的最小总耗时},
$$
其中分数超过 $k$ 的一律记成 $k$（因为只要及格）。

**引理 2（DP 转移正确性）**：初始 $dp[0]=0$，其余 $dp[s]=+\infty$。对每项作业 $(t_i,v_i)$，按 $s=k,k-1,\dots,0$ 的顺序做
$$
dp[\min(k,s+v_i)]\leftarrow\min\bigl(dp[\min(k,s+v_i)],\ dp[s]+t_i\bigr).
$$
处理完全部作业后，$dp[s]$ 就是“得到至少 $s$ 分”的最小耗时。

**证明**：对已处理作业的个数归纳。

- 初始：不选任何作业，得到 $0$ 分，耗时 $0$；其余分数不可达。
- 加入第 $i$ 项作业时，任何使用前 $i$ 项的子集，要么不用第 $i$ 项（对应 $dp[s]$ 原值），要么用第 $i$ 项（对应某个 $dp[s']+t_i$，其中 $s=\min(k,s'+v_i)$）。转移取所有可能的最小值，故归纳成立。
- 逆序枚举 $s$ 保证每项作业至多被用一次（$s$ 只从更小的分数转移而来，而逆序保证读取的 $dp[s]$ 还是“前 $i-1$ 项”的值）。$\blacksquare$

处理完后，$dp[k]$ 就是及格所需的最小耗时。

### 2.4 合并

枚举最终得分状态？不需要：既然 $dp[k]$ 已经是“至少 $k$ 分”的最小耗时，那剩余时间
$$
\text{rem}=r-dp[k]
$$
就是能拿出的最大刷题时间（耗时更少、得分更高的方案不会更差，因为刷题只关心剩余时间）。

用引理 1 在 `rem` 下求最多刷题数即可。

**定理**：输出 `q` 即为答案。

**证明**：任取一个合法方案，它的作业子集得分 $\ge k$，耗时 $\ge dp[k]$，故剩余时间 $\le r-dp[k]=\text{rem}$；由引理 1，它能刷的题数不超过 `q`。另一方面，取达到 $dp[k]$ 的作业子集，剩余时间恰为 `rem`，由引理 1 可刷 `q` 道。故 `q` 可达且最优。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **decompose** — 把问题拆成“作业及格”和“剩余时间刷题”两段。
- **explore its property** — 固定剩余时间时，刷题只需按耗时从小到大取。
- **greedy proof** — 用前缀和的上界/可达性证明取最小的若干道最优。
- **knapsack DP** — 以“分数”为状态，值存“最小耗时”，分数超过 $k$ 截断。
- **merge** — 最优作业方案之后剩余时间最大，故直接用 $r-dp[k]$。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| $n,m,k,r$ | `int` | 刷题数、作业数、及格分、总时间 | 读入 | 规模 |
| `tprob[i]` | `int[15]` | 每道刷题的耗时 | 读入 | problem time |
| `thw[i]` | `int[15]` | 每项作业耗时 | 读入 | homework time |
| `score[i]` | `int[15]` | 每项作业得分 | 读入 | score |
| `dp[s]` | `int[55]` | 拿到至少 $s$ 分的最小耗时 | $dp[0]=0$，其余 $+\infty$ | knapsack |
| `rem` | `int` | 及格后的剩余时间 | $r-dp[k]$ | remaining |
| `used` | `int` | 当前刷题已用时间 | $0$ | used time |

### 4.2 算法步骤

1. 读入 $n,m,k,r$、刷题耗时、作业耗时、作业得分。
2. 初始化 $dp[0]=0$，其余 $dp[s]=+\infty$。
3. 对每项作业 $(thw_i,score_i)$，对 $s=k,k-1,\dots,0$：
   $$
   dp[\min(k,s+score_i)]\leftarrow\min\bigl(dp[\min(k,s+score_i)],dp[s]+thw_i\bigr).
   $$
4. 令 $\text{rem}=r-dp[k]$（若为负则无法及格，输出 $0$；题面保证不会发生）。
5. 把 `tprob` 升序排序，从前往后累加，能放下就计数，得到答案。

### 4.3 复杂度

- 时间：$O(mk+n\log n)$，在本题数据下不超过几百次操作。
- 空间：$O(n+m+k)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1926 小书童——刷题大军
// https://www.luogu.com.cn/problem/P1926
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{15};
constexpr int mxk{55};
constexpr int inf{1145141919};

int n,m,k,r;
int tprob[mxn];
int thw[mxn],score[mxn];
int dp[mxk];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>k>>r;
	for(int i=0;i<n;++i)
		cin>>tprob[i];
	for(int i=0;i<m;++i)
		cin>>thw[i];
	for(int i=0;i<m;++i)
		cin>>score[i];

	fill_n(dp,k+1,inf);
	dp[0]=0;
	for(int i=0;i<m;++i)
		for(int s=k;s>=0;--s)
		{
			if(dp[s]==inf)continue;
			int ns=min(k,s+score[i]);
			dp[ns]=min(dp[ns],dp[s]+thw[i]);
		}

	int rem=r-dp[k];
	if(rem<0)
	{
		cout<<0<<'\n';
		return 0;
	}

	sort(tprob,tprob+n);
	int ans{0},used{0};
	for(int i=0;i<n;++i)
	{
		if(used+tprob[i]<=rem)
		{
			used+=tprob[i];
			++ans;
		}
	}
	cout<<ans<<'\n';
	return 0;
}
```
