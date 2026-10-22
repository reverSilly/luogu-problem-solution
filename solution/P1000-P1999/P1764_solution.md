# P1764 翻转游戏 (加强版)

## 1. 重新读题

给定一个 $n \times n$（$1 \le n \le 16$）的黑白棋盘（`w` 表示白，`b` 表示黑）。每次操作可以选择任意一个格子，将该格子及其上、下、左、右相邻的（至多 $5$ 个）棋子同时翻转颜色。求使棋盘上所有棋子变为同一种颜色（全白或全黑）所需的最少操作次数；若无解输出 `Impossible`。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：操作交换律、幂零性与首行枚举决定全局

点灯游戏（Lights Out）有两条基本代数性质：
1. **交换律**：翻转操作的先后顺序不影响最终状态；
2. **模 $2$ 幂零性**：同一个格子翻转 $2$ 次等价于不翻转，故每个格子至多只需操作 $1$ 次。

如果直接枚举全部 $n^2$ 个格子的操作状态，复杂度为 $2^{256}$，不可行。但观察第 $r$ 行的格子 $(r, c)$：一旦前 $r$ 行的操作已经确定，**唯一还能改变 $(r, c)$ 颜色的后续操作就只有正下方的 $(r+1, c)$**！

**引理 1（首行状态唯一决定后续行定理）**：
固定目标颜色 $T \in \{0, 1\}$。当第 $0$ 行的操作掩码 $\text{op}[0] \in [0, 2^n)$ 确定后，对于每一行 $r = 1, 2, \dots, n-1$，为了让第 $r-1$ 行的所有格子最终都等于目标颜色 $T$，第 $r$ 行的操作掩码 $\text{op}[r]$ 被唯一强制确定为：
$$\text{op}[r] = \text{cur}[r-1] \oplus (T \cdot (2^n - 1)),$$
其中 $\text{cur}[r-1]$ 是第 $r-1$ 行在经历 $\text{op}[r-2]$ 与 $\text{op}[r-1]$ 作用后的当前颜色掩码。递推至最后一行后，该首行掩码 $\text{op}[0]$ 合法当且仅当第 $n-1$ 行的最终颜色掩码恰好全为 $T$。

**证明**：
由于按行从上到下依次决策，当完成第 $0 \dots r-1$ 行的操作后，第 $r-1$ 行的第 $c$ 列格子在后续第 $r \dots n-1$ 行的所有操作中，仅与 $(r, c)$ 相邻。因此：
- 若 $(r-1, c)$ 当前颜色不等于 $T$，则 $(r, c)$ 必须翻转（取 $1$）；
- 若 $(r-1, c)$ 当前颜色已等于 $T$，则 $(r, c)$ 绝不能翻转（取 $0$）。
这唯一确定了 $\text{op}[r]$。当 $\text{op}[0 \dots n-1]$ 全部唯一确定后，前 $n-1$ 行必然全部达标，故整体达标当且仅当最后一行 $\text{cur}[n-1]$ 也全为 $T$。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 利用翻转操作模 $2$ 线性性，发现第 $r-1$ 行的残留不匹配位只能由第 $r$ 行正下方位置补偿（引理 1）。
- **classic problem solution** — 枚举第 $0$ 行的 $2^n$ 种位掩码，用位运算在 $O(n)$ 时间内递推后续 $n-1$ 行并校验末行，分别对全 `0` 和全 `1` 目标取最小操作数。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `int` | 棋盘边长（$\le 16$） | 标准输入 | 维度 |
| `init_r[i]` | `int [20]` | 第 $i$ 行的初始状态位掩码（`b` 为 $1$，`w` 为 $0$） | 读入构建 | initial rows |
| `full` | `int` | 低 $n$ 位全 $1$ 掩码 `(1 << n) - 1` | 计算得出 | bitmask |

### 4.2 算法步骤

1. 读入 $n$ 及 $n$ 行字符串，将每行转为二进制掩码 `init_r[0 .. n-1]`。
2. 对两种目标（全 `0` 或全 `1`）分别枚举首行操作掩码 `first = 0 .. (1 << n) - 1`：
   - 维护上一行残留颜色与当前行操作 `op`，用位运算 `op ^ (op << 1) ^ (op >> 1)` 计算同行翻转影响，累加 `__builtin_popcount(op)`；
   - 若最后一行残留颜色全等于目标，用总翻转数更新 `ans`。
3. 若 `ans == inf` 输出 `Impossible`，否则输出 `ans`。

### 4.3 复杂度分析

- **时间复杂度**：共 $2 \times 2^n$ 种首行状态，每种递推 $n$ 行，总时间复杂度为 $\Theta(n 2^n)$（$n = 16$ 时仅 $2 \times 16 \times 65536 \approx 2.1 \times 10^6$ 次位运算，耗时 $< 5\text{ ms}$）。
- **空间复杂度**：一维掩码数组，空间复杂度为 $\Theta(n)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1764 翻转游戏 (加强版)
// https://www.luogu.com.cn/problem/P1764
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int inf{1145141919};

int n;
int init_r[20];

int solve_target(int target_bit)
{
	int full=(1<<n)-1;
	int target_mask=target_bit?full:0;
	int best=inf;
	for(int first=0;first<=full;++first)
	{
		int cnt=0,op=first,prev_op=0;
		int cur=init_r[0];
		for(int i=0;i<n;++i)
		{
			cnt+=__builtin_popcount(op);
			cur^=(op^(op<<1)^(op>>1))&full;
			if(i+1<n)
			{
				int nxt_cur=init_r[i+1]^op;
				prev_op=op;
				op=cur^target_mask;
				cur=nxt_cur;
			}
		}
		if(cur==target_mask)
			best=min(best,cnt);
	}
	return best;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		string s;
		cin>>s;
		int msk=0;
		for(int j=0;j<n;++j)
			if(s[j]=='b')
				msk|=(1<<j);
		init_r[i]=msk;
	}

	int ans=min(solve_target(0),solve_target(1));
	if(ans==inf)
		cout<<"Impossible\n";
	else
		cout<<ans<<'\n';
	return 0;
}
```
