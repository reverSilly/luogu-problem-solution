# P1964 【mc生存】卖东西

## 1. 重新读题

### 1.1 问题重述（人话版）

背包一共 $21$ 格，初始已经放了 $m$ 件**不同的**物品（占 $m$ 格，不能卖）。现在商店里有 $n$ 种物品，第 $i$ 种：

- 名字 $st_i$（只用于区分/读入，不参与计算）；
- 有 $a_i$ 件可买；
- 单件价值 $b_i$；
- 每格最多放 $c_i$ 件，**同种物品可以叠在一格里**。

问一次最多能买回总价值多少的物品。

### 1.2 关键约束与规模暗示

- $0\le m\le 21$，所以真正可用的空格数 $cap=21-m$ 不超过 $21$。
- $0\le n\le 100$，$a_i\le 1344$，$b_i\le 10^4$，$1\le c_i\le 64$。
- $cap\le 21$ 是关键：无论物品多少，答案只可能用到 $21$ 个“格子”，状态极小。
- 题目明确提示“搜索 0 分，请使用多重背包”，这直接指向按“格子”做背包。
- 答案保证 $s\le 10^6$，`int` 足够。

**题意澄清**：$m$ 件初始物品互不相同，所以各占一格；同种可叠放只针对新买的同种物品。$b_i$ 是“单件价值”，目标是所有买到物品的价值之和，而不是花的钱。

---

## 2. 思维链（Chain-of-thought）

### 2.1 先把“格子”当成背包容量

真正可支配的只有 $cap=21-m$ 个空格。一种物品买 $k$ 件，需要占用的格子数是
$$
\left\lceil \frac{k}{c_i}\right\rceil
$$
（每格放 $c_i$ 件，最后一格可能没放满）。

如果没有“一件物品可以只买一部分、最后一格不必放满”这个细节，问题就是每个格子里放哪种物品、放几件。这个细节说明：**按“整格”划分时，最后一格可以是零头**。

### 2.2 把每种物品拆成“格子的包”

对第 $i$ 种物品：

- 满格包：$\lfloor a_i/c_i\rfloor$ 个，每个占 $1$ 格、装 $c_i$ 件，价值 $c_i b_i$；
- 零头包：若 $r_i=a_i\bmod c_i>0$，再加 $1$ 个，占 $1$ 格、装 $r_i$ 件，价值 $r_i b_i$。

这样每种物品最多 $cap+1$ 个包（超过 $cap$ 个满格包不可能全用上），于是所有物品总共至多 $100\times22$ 个包。

**引理（拆分等价）**：原问题的最优值，等于在“所有包”上做 0/1 背包（每包重量 $1$、容量 $cap$）的最优值。

**证明**：

记每种物品的满格包数为 $F_i=\lfloor a_i/c_i\rfloor$，零头 $r_i=a_i\bmod c_i$。

（一）任意一组包 $\Rightarrow$ 一个合法购买方案。
设第 $i$ 种选了 $f_i$ 个满格包，以及是否选零头包（记 $\epsilon_i\in\{0,1\}$）。令购买件数
$$
k_i=f_i c_i+\epsilon_i r_i .
$$
因为 $f_i\le F_i$，所以 $k_i\le F_i c_i+r_i=a_i$，不超过库存；占用格子数为 $f_i+\epsilon_i$，正好等于选中包数（满格包各占 $1$ 格，零头包占 $1$ 格）；价值为 $f_i c_i b_i+\epsilon_i r_i b_i=k_i b_i$。所以只要总包数 $\le cap$，就是一个合法购买方案，且价值等于包的总价值。故“真实最优值 $\ge$ 背包最优值”。

（二）任意一个合法购买方案 $\Rightarrow$ 价值不超过某组包的价值。
设方案给第 $i$ 种分配 $s_i$ 格（$\sum s_i\le cap$）。由于每格最多 $c_i$ 件且最多买 $a_i$ 件，实际买到件数 $k_i\le\min(a_i,\ s_i c_i)$，价值 $\le \min(a_i,\ s_i c_i)\,b_i$。下面说明用包能凑出**恰好** $\min(a_i,s_i c_i)$ 件、且只花不超过 $s_i$ 格：

- 若 $s_i\le F_i$：选 $s_i$ 个满格包，件数 $s_i c_i=\min(a_i,s_i c_i)$，花 $s_i$ 格。
- 若 $s_i\ge F_i+1$ 且 $r_i>0$：选全部 $F_i$ 个满格包 + 零头包，件数 $F_i c_i+r_i=a_i=\min(a_i,s_i c_i)$，花 $F_i+1\le s_i$ 格。
- 若 $s_i\ge F_i+1$ 且 $r_i=0$：选全部 $F_i$ 个满格包，件数 $a_i=\min(a_i,s_i c_i)$，花 $F_i\le s_i$ 格。

三种情况都只用不超过 $s_i$ 格就拿到不少于原方案的价值。对每种物品独立执行，总格数不超过 $\sum s_i\le cap$。故“真实最优值 $\le$ 背包最优值”。

两向合并，引理成立。$\square$

### 2.3 背包怎么做

所有包的重量都是 $1$，所以就是标准的 0/1 背包：
$$
dp[j]=\text{用至多 } j \text{ 个格子能获得的最大价值},\quad dp[j]=\max(dp[j],dp[j-1]+value).
$$
因为重量全为 $1$，从 $cap$ 到 $1$ 倒序枚举 $j$ 即可保证每个包只用一次。

（另一种等价理解：既然所有包各占一格，答案就是把所有包的“价值”从大到小排序、取前 $cap$ 个之和。背包写法和排序写法都能过；这里用背包，正是题面提示的“多重背包”。）

### 2.4 一些边界

- $m=21$ 时 $cap=0$，什么都放不下，答案 $0$。
- $n=0$ 时没有可买物品，答案 $0$。
- $a_i<c_i$ 时 $F_i=0$，只有零头包，仍然合法（买 $a_i$ 件占 $1$ 格）。
- 超过 $cap$ 的满格包不可能用到，代码中 `take=min(F_i,cap)` 截断，保证包数量级极小。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 容量只有 $cap=21-m\le21$ 格，状态极小。
- **reduce to easier problem** — 把“买若干件、占用若干格”拆成“满格包 + 零头包”，每包恰好占 $1$ 格。
- **prove both directions** — 包装配合法、任意方案都能被包组合不劣地表示。
- **classic problem solution** — 0/1 背包，容量 $cap$，物品是这些“格子包”。

---

## 4. 真实解法（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `m` | `int` | 初始物品数（占 $m$ 格） | 读入 | — |
| `n` | `int` | 物品种类数 | 读入 | — |
| `cap` | `int` | 可用格子数 $21-m$ | 计算 | capacity |
| `a,b,c` | `int` | 库存/单件价值/每格容量 | 每行读入 | — |
| `s` | `string` | 物品名字（读入即弃） | 每行读入 | — |
| `full` | `int` | 满格包数 $\lfloor a/c\rfloor$ | 计算 | — |
| `rem` | `int` | 零头件数 $a\bmod c$ | 计算 | remainder |
| `take` | `int` | 实际加入的满格包数 $\min(full,cap)$ | 计算 | — |
| `dp[j]` | `int [22]` | 用至多 $j$ 格的最大价值 | 0 | DP |

### 4.2 算法步骤

1. 读入 $m,n$，令 `cap=21-m`。
2. 对每种物品读入 $a,b,c,s$：
   1. `full=a/c`，`rem=a%c`，`take=min(full,cap)`；
   2. 把 `take` 个满格包依次加入 0/1 背包：每个重量 $1$、价值 $c*b$；
   3. 若 `rem>0`，再加入一个重量 $1$、价值 `rem*b` 的零头包。
3. 输出 `dp[cap]`。

### 4.3 复杂度分析

- 时间：每种物品最多 $cap+1\le22$ 个包，每个包 $O(cap)$ 更新，总 $O(n\cdot cap^2)\le 100\times 22^2$，可忽略。
- 空间：$O(cap)$，即一个长度 $22$ 的数组。

---

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxcap{22};

int m,n;
int dp[mxcap];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n;
	int cap=21-m;
	for(int i=0;i<n;++i)
	{
		int a,b,c;
		string s;
		cin>>a>>b>>c>>s;
		int full=a/c;
		int rem=a%c;
		int take=min(full,cap);
		for(int k=0;k<take;++k)
			for(int j=cap;j>=1;--j)
				dp[j]=max(dp[j],dp[j-1]+c*b);
		if(rem>0)
			for(int j=cap;j>=1;--j)
				dp[j]=max(dp[j],dp[j-1]+rem*b);
	}
	cout<<dp[cap]<<'\n';
	return 0;
}
```
