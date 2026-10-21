# P1854 [IOI 1999] 花店橱窗布置

> 难度：普及/提高- · 标签：动态规划 DP, 1999, IOI, Special Judge, 记忆化搜索

## 1. 重新读题
### 1.1 问题重述（人话版）

有 $F$ 束花和 $V$ 个从左到右排好的花瓶，$F\le V$。花必须按编号 $1,2,\ldots,F$ 从左到右放：花 $i$ 的花瓶编号 $p_i$ 严格小于花 $i+1$ 的花瓶编号。每个花瓶至多一束花，每束花都必须放进去。花 $i$ 放进花瓶 $j$ 的美学值是 $a_{i,j}$，可正可负。求最大美学值，并输出任意一种达到该值的摆放。

### 1.2 关键约束与规模暗示

$1\le F\le V\le 100$，$-50\le a_{i,j}\le 50$。方案不唯一，按任意正确解比较，不能要求字符串与某一份参考输出逐字相同。状态如果是「前 $i$ 束花、第 $i$ 束放在花瓶 $j$」，转移枚举上一束的花瓶，复杂度 $O(FV^2)\le 10^6$，足够。美学值有负数，初值不能用 $0$ 当「还没算」。

## 2. 思维链（Chain-of-thought）

**引理 1（可行解的形状）。** 一组摆放合法当且仅当存在严格递增下标 $1\le p_1<p_2<\cdots<p_F\le V$，总分为 $\sum_{i=1}^F a_{i,p_i}$。

证明：每个花瓶至多一束，且编号较小的花必须在左边，所以 $p$ 严格递增。反之，严格递增的 $p$ 同时满足顺序和「一瓶一花」，又恰好放了 $F$ 束，故合法。$\blacksquare$

**引理 2（子问题封闭）。** 设 $dp_{i,j}$ 为「花 $1..i$ 都已放下，且花 $i$ 放在花瓶 $j$」的最大美学值（若不可能则为 $-\infty$）。则
$$
dp_{i,j}=a_{i,j}+\max_{i-1\le k<j}dp_{i-1,k},
$$
其中 $j$ 还须留下右边 $F-i$ 个花瓶，即 $i\le j\le V-(F-i)$；边界 $dp_{0,0}=0$，其余 $dp_{0,\cdot}=-\infty$。

证明：花 $i$ 放在 $j$ 时，花 $1..i-1$ 必须全部放在 $1..j-1$ 的花瓶里，且花 $i-1$ 的花瓶 $k$ 满足 $k<j$。这些花自己也要合法，所以 $k\ge i-1$。前 $i-1$ 束的最优摆放与第 $i$ 束的 $a_{i,j}$ 相加，没有交叉项，因此最优解的前缀也是对应子问题的最优解：若前缀不是最优，换成最优前缀会让总分变大，矛盾。右边至少留 $F-i$ 个空瓶，否则后面的花无处可放，这些 $j$ 直接不进入状态。$i=1$ 时前缀只有空方案 $dp_{0,0}$。$\blacksquare$

**引理 3（答案与方案）。** 最大美学值是 $\max_j dp_{F,j}$。从取到最大的 $j$ 出发，顺着转移时记下的前驱 $k$ 倒推，得到的 $p$ 合法且总分等于该最大值。

证明：任何完整方案的最后一束都落在某个花瓶 $j$ 上，其分数就是某个 $dp_{F,j}$，所以最大值不会漏。倒推时每一步 $k<j$ 且 $dp_{i-1,k}$ 被选中，归纳可得下标严格递增、每束花都出现一次，且沿途加起来的美学值等于 $dp_{F,j}$。$\blacksquare$

负数不会破坏上述论证：$dp$ 用 $-\infty$ 初始化，比较用严格大于，未到达的状态不会冒充 $0$。

## 3. 模式总结（Pattern Summary）

- **restate as increasing indices** — 顺序限制就是严格递增的花瓶下标
- **cut at the last flower** — 子问题固定「第 $i$ 束放在 $j$」
- **enumerate the previous vase** — 转移只依赖更左边的 $dp_{i-1,k}$
- **reject states that leave no room** — $j$ 右端至少留 $F-i$ 个花瓶
- **walk predecessors backward** — 记录前驱即可还原任意最优方案

## 4. 真实解法
### 4.1 符号表（表格：符号 / 类型尺寸 / 含义 / 初值 / 助记）

| 符号 | 类型尺寸 | 含义 | 初值 | 助记 |
| --- | --- | --- | --- | --- |
| $f,v$ | `int` | 花束数、花瓶数 | 读入 | 题面 $F,V$ |
| `a[105][105]` | `int` | 美学值，1-based | 读入 | $a_{i,j}$ |
| `dp[105][105]` | `int` | 引理 2 的状态 | `-inf` | 前 $i$ 束、末瓶 $j$ |
| `pre[105][105]` | `int` | 花 $i$ 放 $j$ 时花 $i-1$ 的花瓶 | $0$ | 倒推指针 |
| `anspos[105]` | `int` | 输出的 $p_i$ | $0$ | 正序答案 |
| `inf` | `constexpr int` | 哨兵 $1145141919$ | 常量 | $dp$ 取负哨兵 |

### 4.2 算法步骤

1. 读入 $F,V$ 和矩阵。把 `dp` 全部设成 `-inf`，`dp[0][0]=0`。
2. 对 $i=1..F$、$j=i..V-(F-i)$、$k=i-1..j-1$，若 `dp[i-1][k]` 可达，用 `dp[i-1][k]+a[i][j]` 更新 `dp[i][j]`，并记下 `pre[i][j]=k`。
3. 在 $j=F..V$ 中取 `dp[F][j]` 最大的位置，倒推得到 $p_F,\ldots,p_1$，再正序输出最大值和 $p$。

### 4.3 复杂度与验证状态（只写你真实跑出来的数字）

时间 $O(FV^2)$，空间 $O(FV)$。

本地 `g++ -O2 -std=c++17 -Wall -Wextra` 无警告。对拍脚本先跑题面样例（输入为 $3\ 5$ 的那张美学表），再跑 $F=1$、全负、方阵等边界和 $80$ 组 $F\le 7,V\le 12$ 的随机数据：暴力枚举组合只输出最优值，标程输出值和方案；方案被检查为严格递增且美学和等于该最优值。结果：

- `86 tests OK`
- 最大数据 $F=V=100$、美学值在 $[-50,50]$ 随机，标程墙钟 `0.009969s`，方案自洽（严格递增且和等于输出值）

样例标程输出 `53` 与 `2 4 5`，暴力最优值 `53`。

## 5. 示例代码
### 5.1 标程（`P1854.cpp`）

```cpp
//
// 题目：P1854 [IOI 1999] 花店橱窗布置
// https://www.luogu.com.cn/problem/P1854
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};
constexpr int inf{1145141919};

int f,v;
int a[mxn][mxn];
int dp[mxn][mxn];
int pre[mxn][mxn];
int anspos[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>f>>v;
	for(int i=1;i<=f;++i)
		for(int j=1;j<=v;++j)
			cin>>a[i][j];

	for(int i=0;i<=f;++i)
		for(int j=0;j<=v;++j)
			dp[i][j]=-inf;
	dp[0][0]=0;

	for(int i=1;i<=f;++i)
	{
		for(int j=i;j<=v-(f-i);++j)
		{
			for(int k=i-1;k<j;++k)
			{
				if(dp[i-1][k]==-inf)
					continue;
				int cur=dp[i-1][k]+a[i][j];
				if(cur>dp[i][j])
				{
					dp[i][j]=cur;
					pre[i][j]=k;
				}
			}
		}
	}

	int best=-inf,bj=0;
	for(int j=f;j<=v;++j)
		if(dp[f][j]>best)
		{
			best=dp[f][j];
			bj=j;
		}

	for(int i=f;i>=1;--i)
	{
		anspos[i]=bj;
		bj=pre[i][bj];
	}

	cout<<best<<'\n';
	for(int i=1;i<=f;++i)
	{
		if(i>1)
			cout<<' ';
		cout<<anspos[i];
	}
	cout<<'\n';
	return 0;
}
```
