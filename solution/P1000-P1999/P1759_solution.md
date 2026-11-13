# P1759 通天之潜水

## 1. 重新读题

有 $n$（$1 \le n \le 100$）件潜水工具，第 $i$ 件工具的重量为 $a_i$、阻力为 $b_i$、可支撑时间为 $c_i$。背包的最大承重为 $m$，最大可承受阻力为 $v$（$1 \le m, v \le 200$）。

求在总重量 $\le m$ 且总阻力 $\le v$ 的前提下，所能获得的最大总支撑时间，并在第二行输出所选工具的编号序列（若存在多种最大时间方案，输出**字典序最小**的编号序列）。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：二维费用 0-1 背包与最小字典序方案还原

这是一个标准的**二维费用 0-1 背包**问题，难点在于“当最大价值相同时，如何保证输出的物品编号序列字典序最小”。

**引理 1（倒序 DP 正推还原最小字典序原理）**：
将物品按编号从后往前（$i = n, n-1, \dots, 1$）进行 DP，定义 $dp[i][j][k]$ 为从后缀物品 $\{i, i+1, \dots, n\}$ 中选取、总重量 $\le j$、总阻力 $\le k$ 时的最大时间。
在正序还原（$i = 1, 2, \dots, n$）时，若当前剩余容量 $(j, k)$ 满足 $j \ge a_i \land k \ge b_i$ 且
$$dp[i][j][k] == dp[i+1][j - a_i][k - b_i] + c_i,$$
则**必须立即贪心选上物品 $i$**（并将剩余容量更新为 $(j - a_i, k - b_i)$）；如此依次决策得到的必是字典序最小的最优方案。

**证明**：
字典序比较由第一个出现差异的最小编号决定：包含编号更小物品 $i$ 的方案，其第一个元素为 $i$，严格小于任何跳过 $i$ 而只从 $\{i+1, \dots, n\}$ 中选取的方案的对应元素。
由于 $dp[i+1][j - a_i][k - b_i] + c_i = dp[i][j][k]$ 保证了在选上物品 $i$ 后，剩余后缀 $\{i+1, \dots, n\}$ 依然存在至少一种能凑出最大剩余价值 $dp[i][j][k] - c_i$ 的合法方案，因此“只要能选 $i$ 且不损失最优值就必选 $i$”在每一步都做出了字典序最优且后续可行的决策，由归纳法知最终序列字典序全局最小。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 识别出重量与阻力双维度的 0-1 背包模型。
- **classic problem solution** — 按编号倒序（$n \to 1$）计算后缀背包 $dp[i][j][k]$，再按编号正序（$1 \to n$）能选则选地还原出字典序最小的最优方案（引理 1）。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `m, v, n` | `int` | 最大承重、最大阻力、工具总数 | 标准输入 | 维度 |
| `a[i], b[i], c[i]` | `int [105]` | 第 $i$ 件工具的重量、阻力、时间 | 标准输入 | item attributes |
| `dp[i][j][k]` | `int [105][205][205]` | 后缀 $i \dots n$ 在容量 $(j, k)$ 下的最大时间 | 全 `0` | suffix DP |

### 4.2 算法步骤

1. 读入 $m, v, n$ 及 $n$ 件物品的 $(a_i, b_i, c_i)$。
2. 倒序枚举 $i = n, n-1, \dots, 1$，枚举 $j = 0 \dots m$、$k = 0 \dots v$：
   - 令 `dp[i][j][k] = dp[i+1][j][k]`；
   - 若 $j \ge a_i$ 且 $k \ge b_i$，用 `dp[i+1][j - a[i]][k - b[i]] + c[i]` 更新 `dp[i][j][k]` 的最大值。
3. 第一行输出 `dp[1][m][v]`。
4. 令当前容量 `cj = m, ck = v`，正序枚举 $i = 1 \dots n$：若 `cj >= a[i] && ck >= b[i] && dp[i][cj][ck] == dp[i+1][cj - a[i]][ck - b[i]] + c[i]`，输出 $i$ 并更新 `cj -= a[i], ck -= b[i]`。

### 4.3 复杂度分析

- **时间复杂度**：状态总数 $n \cdot m \cdot v \le 100 \times 200 \times 200 = 4 \times 10^6$，转移 $O(1)$，时间复杂度为 $\Theta(nmv)$（耗时 $< 15\text{ ms}$）。
- **空间复杂度**：三维数组 `dp[105][205][205]` 占用约 $17\text{ MB}$，空间复杂度为 $\Theta(nmv)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1759 通天之潜水
// https://www.luogu.com.cn/problem/P1759
// AC
#include<bits/stdc++.h>
using namespace std;

int m,v,n;
int a[105],b[105],c[105];
int dp[105][205][205];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>v>>n;
	for(int i=1;i<=n;++i)
		cin>>a[i]>>b[i]>>c[i];

	for(int i=n;i>=1;--i)
		for(int j=0;j<=m;++j)
			for(int k=0;k<=v;++k)
			{
				dp[i][j][k]=dp[i+1][j][k];
				if(j>=a[i]&&k>=b[i])
					dp[i][j][k]=max(dp[i][j][k],dp[i+1][j-a[i]][k-b[i]]+c[i]);
			}

	cout<<dp[1][m][v]<<'\n';
	int cj=m,ck=v;
	bool first=true;
	for(int i=1;i<=n;++i)
		if(cj>=a[i]&&ck>=b[i]&&dp[i][cj][ck]==dp[i+1][cj-a[i]][ck-b[i]]+c[i])
		{
			if(!first)
				cout<<' ';
			first=false;
			cout<<i;
			cj-=a[i];
			ck-=b[i];
		}
	cout<<'\n';
	return 0;
}
```
