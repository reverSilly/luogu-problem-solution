# P1799 数列

## 1. 重新读题

给定一个长度为 $n$（$1 \le n \le 10^3$）的正整数序列 $A_1, A_2, \dots, A_n$。可以擦掉其中的任意多个数，擦掉后剩余元素按原顺序向左靠拢重新从 $1$ 开始编号。求擦掉若干数后，最多能有多少个元素满足“数值等于其所在的新下标”。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：保留个数与目标下标的匹配条件

对于原序列中第 $i$ 个元素 $A_i$，要想让它在擦掉若干元素后恰好落在第 $A_i$ 个位置上，意味着在 $1 \dots i$ 中**必须恰好保留了 $A_i$ 个元素**（或者说在前 $i-1$ 个元素中恰好擦掉了 $i - A_i$ 个元素）！

**引理 1（按保留元素个数设计的二维 DP）**：
设 $dp[i][j]$ 表示考虑原序列的前 $i$ 个元素、从中**保留了恰好 $j$ 个元素**（$0 \le j \le i$）时，最多能有多少个元素落在自己的位置上。则对第 $i$ 个元素有两种决策：
1. **擦掉 $A_i$**：保留个数仍为 $j$，转移自 $dp[i-1][j]$；
2. **保留 $A_i$**：它是第 $j$ 个被保留的元素（新下标为 $j$），若 $A_i == j$ 则匹配数 $+1$，转移自 $dp[i-1][j-1] + [A_i == j]$。
合并得状态转移方程：
$$dp[i][j] = \max\big( dp[i-1][j],\ dp[i-1][j-1] + [A_i == j] \big).$$

**证明**：新序列中每个被保留元素的最终下标只取决于在它之前（含自身）一共保留了多少个元素，与具体保留了哪些元素无关，满足无后效性与最优子结构。倒序枚举 $j = i \dots 1$ 即可将空间压为一维数组 `dp[j]`。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 原下标 $i$ 处的元素 $A_i$ 匹配新位置当且仅当在 $1 \dots i$ 中恰好保留了 $j = A_i$ 个元素（引理 1）。
- **classic problem solution** — 倒序更新一维数组 `dp[j] = max(dp[j], dp[j-1] + (a[i] == j))`，时间复杂度 $\Theta(n^2)$，空间复杂度 $\Theta(n)$。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `int` | 序列长度（$\le 1000$） | 标准输入 | length |
| `dp[j]` | `int [1005]` | 当前前缀中保留 $j$ 个元素时的最大匹配数 | 全 `0` | DP array |

### 4.2 算法步骤

1. 读入 $n$。
2. 枚举 $i = 1 \dots n$，读入 $x = A_i$，倒序枚举保留个数 $j = i \dots 1$：令 `dp[j] = max(dp[j], dp[j - 1] + (x == j))`。
3. 输出 $\max_{0 \le j \le n} dp[j]$。

### 4.3 复杂度分析

- **时间复杂度**：双重循环共 $\frac{n(n+1)}{2} \approx 5 \times 10^5$ 次操作，时间复杂度为 $\Theta(n^2)$（耗时 $< 2\text{ ms}$）。
- **空间复杂度**：一维数组长度 $1005$，空间复杂度为 $\Theta(n)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1799 数列
// https://www.luogu.com.cn/problem/P1799
// AC
#include<bits/stdc++.h>
using namespace std;

int n;
int dp[1005];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1,x;i<=n;++i)
	{
		cin>>x;
		for(int j=i;j>=1;--j)
			dp[j]=max(dp[j],dp[j-1]+(x==j));
	}

	int ans=0;
	for(int j=0;j<=n;++j)
		ans=max(ans,dp[j]);
	cout<<ans<<'\n';
	return 0;
}
```
