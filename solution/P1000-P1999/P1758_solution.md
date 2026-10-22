# P1758 [NOI2009] 管道取珠

## 1. 重新读题

上下两个管道分别装有 $n$ 个和 $m$ 个小球（$1 \le n, m \le 500$），球只有 `A`、`B` 两种颜色。每次操作可选择上管道或下管道，将其最右侧的球推入输出管道。共有 $\binom{n+m}{m}$ 种操作序列。

设所有可能生成的不同输出序列共有 $K$ 种，其中第 $i$ 种输出序列可由 $a_i$ 种不同的操作序列产生。求 $\sum_{i=1}^K a_i^2 \bmod 1024523$。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：平方和 $\sum a_i^2$ 的组合意义转化

由于不同输出序列总数 $K$ 可达指数级，我们绝不可能分别算出每个 $a_i$ 再平方求和！
遇到“方案数的平方和 $\sum a_i^2$”，最经典也最强大的工具是**组合意义转化（算两次 / 双人同步模拟）**：

**引理 1（平方和的双人同步博弈等价性）**：
$\sum_{i=1}^K a_i^2$ 恰好等于：让甲、乙两个人各自独立进行一次游戏，最终**甲和乙产生完全相同的输出序列**的有序操作对 $(S_{\text{甲}}, S_{\text{乙}})$ 的总数！

**证明**：
设产生第 $i$ 种输出序列 $T_i$ 的合法操作序列集合为 $\mathcal{A}_i$，则 $|\mathcal{A}_i| = a_i$，且不同 $i$ 的 $\mathcal{A}_i$ 互不相交。
甲、乙两人得到相同输出序列 $T_i$，当且仅当 $S_{\text{甲}} \in \mathcal{A}_i$ 且 $S_{\text{乙}} \in \mathcal{A}_i$，这样的有序对共有 $|\mathcal{A}_i| \times |\mathcal{A}_i| = a_i^2$ 对。对所有 $i = 1 \dots K$ 求和，总有序对数恰为 $\sum_{i=1}^K a_i^2$。$\blacksquare$

### 2.2 状态压缩与滚动数组 DP

为方便处理，先把上下管道的字符串翻转，使“每次取最右侧的球”变成“按 $1 \dots n$ 和 $1 \dots m$ 从前往后取球”。
在同步走完 $t$ 步后：
- 甲在上管道取了 $i$ 个球，在下管道取了 $j = t - i$ 个球；
- 乙在上管道取了 $k$ 个球，在下管道取了 $l = i + j - k$ 个球。

可见 $l$ 完全由 $(i, j, k)$ 唯一决定！因此状态只需三维 $(i, j, k)$，再将第一维 $i$ 用滚动数组压缩，空间仅需 $O(n \cdot m)$，时间仅需 $\Theta(n^2 m)$！

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 将输出序列频数的平方和 $\sum a_i^2$ 转化为“两人独立操作产生完全相同输出序列的方案对数”（引理 1）。
- **reduce state dimension** — 利用两人总步数相同（$i + j = k + l$）消去第四维 $l = i + j - k$。
- **classic problem solution** — 对状态 $dp[i][j][k]$ 的第一维 $i$ 使用滚动数组，在 $\Theta(n^2 m)$ 时间、$\Theta(nm)$ 空间内完成递推。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, m` | `int` | 上、下管道球数 | 标准输入 | 维度 |
| `a, b` | `string` | 翻转后的上、下管道字符串（1-based） | 读入后 `reverse` | strings |
| `dp[j][k], ndp[j][k]` | `int [mxn][mxn]` | 滚动数组：甲取 $(i, j)$、乙取 $(k, i+j-k)$ 且输出相同的方案数 | `dp[0][0] = 1` | rolling DP |

### 4.2 算法步骤

1. 读入 $n, m$ 及两行字符串，将两字符串翻转（使下标 $1$ 对应最先取出的右侧球）。
2. 初始化 `dp[0][0] = 1`。
3. 外层枚举 $i = 0 \dots n$，中层枚举 $j = 0 \dots m$，内层枚举 $k = 0 \dots n$（令 $l = i + j - k$，若 $l < 0$ 或 $l > m$ 或 `dp[j][k] == 0` 则跳过）：
   - 考察甲下一步选上（$a_{i+1}$）或选下（$b_{j+1}$），乙下一步选上（$a_{k+1}$）或选下（$b_{l+1}$）的 $4$ 种组合：只要两人取出的字符相等，就将 `dp[j][k]` 累加到目标状态（甲选上则累加进下一层 `ndp`，甲选下则累加进当前层 `dp[j+1]`）。
4. 若 $i < n$，将 `ndp` 拷回 `dp` 并清空 `ndp`；当 $i = n$ 处理完毕后输出 `dp[m][n]`。

### 4.3 复杂度分析

- **时间复杂度**：三重循环共 $(n+1)(m+1)(n+1) \approx 1.25 \times 10^8$ 次简单加法取模，耗时约 $80\text{ ms}$。
- **空间复杂度**：两个 $505 \times 505$ 的二维数组，空间复杂度为 $\Theta(nm)$（约 $2\text{ MB}$）。

---

## 5. 示例代码

```cpp
//
// 题目：P1758 [NOI2009] 管道取珠
// https://www.luogu.com.cn/problem/P1758
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{505};
constexpr int mod{1024523};

int n,m;
string a,b;
int dp[mxn][mxn],ndp[mxn][mxn];

void add_mod(int&x,int y)
{
	x+=y;
	if(x>=mod)
		x-=mod;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>a>>b;
	reverse(a.begin(),a.end());
	reverse(b.begin(),b.end());
	a=" "+a;
	b=" "+b;

	dp[0][0]=1;
	for(int i=0;i<=n;++i)
	{
		memset(ndp,0,sizeof(ndp));
		for(int j=0;j<=m;++j)
			for(int k=0;k<=n;++k)
			{
				int v=dp[j][k];
				if(!v)
					continue;
				int l=i+j-k;
				if(l<0||l>m)
					continue;
				if(i<n&&k<n&&a[i+1]==a[k+1])
					add_mod(ndp[j][k+1],v);
				if(i<n&&l<m&&a[i+1]==b[l+1])
					add_mod(ndp[j][k],v);
				if(j<m&&k<n&&b[j+1]==a[k+1])
					add_mod(dp[j+1][k+1],v);
				if(j<m&&l<m&&b[j+1]==b[l+1])
					add_mod(dp[j+1][k],v);
			}
		if(i<n)
			memcpy(dp,ndp,sizeof(dp));
	}
	cout<<dp[m][n]<<'\n';
	return 0;
}
```
