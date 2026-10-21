# P1855 榨取kkksc03

> 难度：普及- · 标签：动态规划 DP, 数学, 2014, 洛谷原创, 枚举, 背包 DP, 洛谷比赛

## 1. 重新读题
### 1.1 问题重述（人话版）

有 $n$ 个愿望，第 $i$ 个要同时花 $m_i$ 元和 $t_i$ 分钟。手里最多 $M$ 元、$T$ 分钟，每个愿望至多做一次。问最多能完成几个愿望。

### 1.2 关键约束与规模暗示

$1\le n\le 100$，$0\le M,T\le 200$。这是两个容量、价值全是 $1$ 的 $0/1$ 背包。$O(nMT)\le 100\cdot200\cdot200=4\cdot10^6$。$M$ 或 $T$ 可以是 $0$，费用也可以是 $0$（免费愿望）。费用超过对应容量的愿望永远不能选。

## 2. 思维链（Chain-of-thought）

**引理 1（模型）。** 答案等于
$$
\max\bigl\{|S|:S\subseteq\{1..n\},\ \sum_{i\in S}m_i\le M,\ \sum_{i\in S}t_i\le T\bigr\}.
$$
证明：题面要求在两种资源都不超限的前提下最大化完成个数，每个愿望最多一次，就是这个集合的基数。$\blacksquare$

**引理 2（二维 $0/1$ 转移）。** 令 $dp_{j,k}$ 表示恰好用掉不超过 $j$ 元、不超过 $k$ 分钟时的最大愿望数（滚动掉「前几个愿望」这一维，且每个愿望只逆序扫一次）。处理愿望 $(c,t)$ 时，对 $j=M..c$、$k=T..t$，
$$
dp_{j,k}=\max(dp_{j,k},\ dp_{j-c,k-t}+1).
$$
最终答案是 $dp_{M,T}$。

证明：逆序保证 $dp_{j-c,k-t}$ 仍是「还没放入当前愿望」的值，所以每个愿望至多进背包一次。$+1$ 对应价值为件数。$j$ 从 $c$ 开始，付不起的状态保持原值，等于不选。二维同时逆序时，即使 $c=0$ 或 $t=0$，被读取的那一格也还没被当前愿望写过：另一维若为正就沿减小方向读旧格；两维都是 $0$ 时读到的是同一格的旧值，只加一次 $1$。归纳：处理完前 $i$ 个愿望后，$dp_{j,k}$ 是只用这 $i$ 个愿望、花费不超过 $(j,k)$ 的最大件数。初始全 $0$ 表示一件都不做。$\blacksquare$

空方案的分数是 $0$，全局数组初值就是 $0$，不必再设哨兵。

## 3. 模式总结（Pattern Summary）

- **two budgets, value one** — 钱和时间都是容量，件数是价值
- **0-1 so scan backward** — 逆序避免一件愿望被加多次
- **zero cost still once** — 费用为 $0$ 时读的仍是旧状态
- **skip if cannot pay** — 循环下界就是费用，超预算自然不转移
- **answer sits at full capacity** — $dp_{M,T}$ 已经是「不超过」的最大值

## 4. 真实解法
### 4.1 符号表（表格：符号 / 类型尺寸 / 含义 / 初值 / 助记）

| 符号 | 类型尺寸 | 含义 | 初值 | 助记 |
| --- | --- | --- | --- | --- |
| `n,mny,tim` | `int` | 愿望数、钱、时间 | 读入 | $n,M,T$ |
| `dp[205][205]` | `int` | 两种资源都不超限的最大件数 | 全局 $0$ | 背包 |
| `c,t` | `int` | 当前愿望的钱和时间 | 读入 | 物品费用 |

### 4.2 算法步骤

1. 读入 $n,M,T$。
2. 对每个愿望，若 $c\le M$ 且 $t\le T$，按 $j$ 从 $M$ 降到 $c$、$k$ 从 $T$ 降到 $t$ 做引理 2 的更新。
3. 输出 `dp[M][T]`。

### 4.3 复杂度与验证状态（只写你真实跑出来的数字）

时间 $O(nMT)$，空间 $O(MT)$。

`g++ -O2 -std=c++17 -Wall -Wextra` 无警告。暴力是 $n\le 16$ 的子集枚举，与背包思路不同。对拍包含官方样例、全 $0$ 容量、免费愿望、费用超过容量，以及 $70$ 组随机。结果：

- `76 tests OK`，样例双方都输出 `4`
- 最大数据 $n=100,M=T=200$，标程墙钟 `0.018777s`，输出 `6`（该组随机的答案，只作计时）

## 5. 示例代码
### 5.1 标程（`P1855.cpp`）

```cpp
//
// 题目：P1855 榨取kkksc03
// https://www.luogu.com.cn/problem/P1855
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxm{205};
constexpr int mxn{105};

int n,mny,tim;
int dp[mxm][mxm];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>mny>>tim;
	for(int i=0;i<n;++i)
	{
		int c,t;
		cin>>c>>t;
		if(c>mny||t>tim)
			continue;
		for(int j=mny;j>=c;--j)
			for(int k=tim;k>=t;--k)
				dp[j][k]=max(dp[j][k],dp[j-c][k-t]+1);
	}
	cout<<dp[mny][tim]<<'\n';
	return 0;
}
```
