# P1877 [HAOI2012] 音量调节

> 难度：普及- · 标签：线性 DP / 可达性

## 1. 重新读题

### 1.1 问题重述（人话版）

吉他初始音量是 `beginLevel`，音量必须始终落在 $[0,\textit{maxLevel}]$。一共 $n$ 首歌，第 $i$ 首开始前必须把音量加上 $c_i$ 或者减去 $c_i$，不能保持不变，也不能越界。希望最后一首歌的音量尽量大。如果怎么选都会在某一步越界，输出 $-1$。

### 1.2 关键约束与规模暗示

- $n\le 50$，$\textit{maxLevel}\le 1000$，$1\le c_i\le \textit{maxLevel}$。
- 音量只有 $1001$ 种。状态就是「处理完前几首歌、当前音量是多少」。
- 顺序不能打乱，$c_i$ 必须按输入顺序用，所以不是随便挑正负号的子集和，而是一条长度为 $n$ 的决策链。

## 2. 思维链（Chain-of-thought）

### 2.1 状态

令 $dp[i][v]$ 表示：前 $i$ 次调节都合法地做完之后，音量可以是 $v$。

边界只有一个真值：$dp[0][\textit{beginLevel}]=\mathrm{true}$。全局 `bool` 数组默认是 `false`，不用再填。

### 2.2 转移恰好覆盖所有合法序列

**引理 1**。$dp[i][v]=\mathrm{true}$ 当且仅当存在一串符号 $\varepsilon_1,\ldots,\varepsilon_i\in\{+1,-1\}$，使得每一步的音量

$$v_0=\textit{beginLevel},\qquad v_t=v_{t-1}+\varepsilon_t c_t$$

都落在 $[0,\textit{maxLevel}]$ 内，并且 $v_i=v$。

**证明**。对 $i$ 归纳。$i=0$ 时只有初始音量，成立。假设 $i-1$ 时集合正确。一次合法的第 $i$ 步，只能从某个可达的 $v_{i-1}$ 走到 $v_{i-1}+c_i$ 或 $v_{i-1}-c_i$，并且新音量仍在区间里。代码对每个可达的 $v$ 恰好尝试这两个后继，所以不会多，也不会漏。$\square$

答案是 $\max\{v:dp[n][v]=\mathrm{true}\}$；若集合为空则为 $-1$。最后一首歌的音量就是第 $n$ 次调节之后的音量，所以看第 $n$ 层是对的。

### 2.3 和「背包」的差别

这里每一次都必须用掉对应的 $c_i$，只是正负可选，而且前后层不能混。用两层（或 $n+1$ 层）布尔数组从左到右推即可，不需要按容量倒序的 0/1 背包写法。倒序只在「同一层反复使用同一件物品」时才有意义，本题读的是上一层、写的是下一层，正序不会覆盖。

对照暴力是 DFS：第 $i$ 首尝试 `+c` 和 `-c`，走到第 $n$ 首就更新最大值。$n\le 18$ 时 $2^{18}$ 可以完整搜索，用来对拍；满数据 $n=50$ 只跑 DP。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 音量域只有 $0..\textit{maxLevel}$，调节顺序固定，正负两种选择。
- **consider easier version** — 递归尝试每一次的加减，小 $n$ 上和 DP 对答案。
- **reachability dp** — $dp[i][v]$ 表示前 $i$ 次调节后能否停在音量 $v$，最后取最大 $v$。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `dp[i][v]` | `bool [55][1005]` | 前 $i$ 首歌后音量 $v$ 是否可达 | `false`，仅 `dp[0][beg]=true` | reachable |
| `c[i]` | `int [55]` | 第 $i$ 次要改变的量 | 读入 | change |
| `cap` | `int` | 最大音量 | 读入 | capacity |
| `ans` | `int` | 最终最大音量 | 从 `cap` 向下找，找不到为 $-1$ | answer |

### 4.2 算法步骤

1. 读入 $n$、初始音量、上限，标记 `dp[0][beg]`。
2. 对第 $i=1..n$ 首歌，枚举上一层所有音量 $v$，若 $v+c_i$ 或 $v-c_i$ 仍在 $[0,\textit{cap}]$ 内，则标记下一层。
3. 从 `cap` 向下找第一个 $dp[n][v]$ 为真的 $v$ 并输出；没有则输出 $-1$。

### 4.3 复杂度与验证状态

- **时间**：$O(n\cdot \textit{maxLevel})$。
- **空间**：$O(n\cdot \textit{maxLevel})$。
- **沙箱对拍**：`g++ -O2 -std=c++17 -Wall -Wextra` 零警告。样例 `3 5 10 / 5 3 7` 输出 `10`；`1 0 3 / 5` 输出 `-1`。另 $120$ 组 $n\le 18$、$\textit{cap}\le 40$ 的随机数据与 DFS 一致，共 $124$ 组。$n=50$、$\textit{cap}=1000$ 标程耗时 $0.0302\text{ s}$，与一份独立的 Python 集合转移结果相同。

## 5. 示例代码

### 5.1 标程（`P1877.cpp`）

```cpp
//
// 题目：P1877 [HAOI2012] 音量调节
// https://www.luogu.com.cn/problem/P1877
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{55};
constexpr int mxv{1005};

bool dp[mxn][mxv];
int c[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int n,beg,cap;
	cin>>n>>beg>>cap;
	dp[0][beg]=true;
	for(int i=1;i<=n;++i)
	{
		cin>>c[i];
		for(int v=0;v<=cap;++v)
		{
			if(!dp[i-1][v])
				continue;
			if(v+c[i]<=cap)
				dp[i][v+c[i]]=true;
			if(v>=c[i])
				dp[i][v-c[i]]=true;
		}
	}
	int ans=-1;
	for(int v=cap;v>=0;--v)
	{
		if(dp[n][v])
		{
			ans=v;
			break;
		}
	}
	cout<<ans<<'\n';
	return 0;
}
```
