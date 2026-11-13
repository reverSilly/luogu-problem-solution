# P1777 [ICPC 2009 Wuhan R] 帮助

## 1. 重新读题

书架上有 $n$ 本书排成一排（$1 \le k \le n \le 100$），每本书的高度是 $25 \dots 32$ 之间的整数（共至多 $8$ 种不同高度，离散化为 $0 \dots 7$）。定义书架的“混乱度”为**连续相同高度书本的极长段数**。

最多可以从中抽走 $k$ 本书，再将这至多 $k$ 本书任意插回书架的任意位置。求操作后能达到的最小混乱度。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：“抽走再插回”等价于“保留子序列 + 缺失颜色补偿”

如果我们固定了**不被抽走**的书本子序列 $B$（至少包含 $n - k$ 本书），设 $B$ 自身的连续同高度段数为 $s$，$B$ 中出现过的高度集合为 $\text{mask} \subseteq \{0, \dots, 7\}$，而原序列所有书本出现过的总高度集合为 $\text{all\_mask}$。
现在考虑把被抽走的书一本一本插回 $B$ 中：

**引理 1（抽走书本的最优插回代价定理）**：
对于任意被抽走的高度为 $h$ 的书：
1. 若 $h \in \text{mask}$（即保留子序列 $B$ 中原本就存在至少一本高度为 $h$ 的书），我们可以把所有被抽走的高度为 $h$ 的书全部紧挨着那本未抽走的 $h$ 插入，**段数增加 $0$**！
2. 若 $h \notin \text{mask}$（即原书架上所有高度为 $h$ 的书都被抽走了），我们必须把这些高度为 $h$ 的书绑在一起插到某处，这会使**总段数恰好增加 $1$**！
因此，固定保留子序列 $B$ 后的最小最终混乱度精确等于：
$$s + \operatorname{popcount}(\text{all\_mask} \setminus \text{mask}).$$

**证明**：
- 每种在原序列中存在（$h \in \text{all\_mask}$）但未在 $B$ 中保留（$h \notin \text{mask}$）的高度 $h$，插回后至少在最终序列中占据 $1$ 个连续段，且不同高度的段互不相交，故段数至少为 $s + |\text{all\_mask} \setminus \text{mask}|$。
- 反之，将每个 $h \in \text{mask}$ 的被抽走书紧贴 $B$ 中已有的 $h$ 插入不产生新段；将每个 $h \in \text{all\_mask} \setminus \text{mask}$ 的所有被抽走书聚成一段插在序列末尾，恰好只新增 $|\text{all\_mask} \setminus \text{mask}|$ 段，达到下界。$\blacksquare$

### 2.2 状压 DP 设计

设 `dp[j][mask][last]` 表示处理完前 $i$ 本书、已抽走 $j \in [0, k]$ 本书、保留书本的高度集合为 `mask` $\in [0, 255]$、且最后一本保留书的高度为 `last` $\in [0, 8]$（$8$ 表示尚未保留任何书）时，保留子序列的最小段数 $s$。
对第 $i$ 本书（高度 $h$）：
1. **抽走它**（若 $j + 1 \le k$）：转移到 `(j + 1, mask, last)`，段数不变；
2. **保留它**：转移到 `(j, mask | (1 << h), h)`，段数增加 `[h != last]`。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 将“抽走再插回”转化为“选长度 $\ge n-k$ 的保留子序列，再加被彻底抽空的高度种类数”（引理 1）。
- **classic problem solution** — 利用高度种类仅 $8$ 种的条件，用滚动数组状压 DP `dp[j][mask][last]` 在 $O(n k \cdot 2^8 \cdot 8)$ 时间内求解。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, k` | `int` | 书本总数与最大抽走数 | 每组读入 | 维度 |
| `all_mask` | `int` | 原序列所有出现过的高度位掩码 | 读入统计 | total heights |
| `dp[j][msk][las]` | `int [105][256][9]` | 抽走 $j$ 本、保留集合 `msk`、末尾 `las` 的最小段数 | `dp[0][0][8]=0` | bitmask DP |

### 4.2 算法步骤

1. 循环读入 $n, k$（$n = k = 0$ 结束），将每本书高度减去 $25$ 映射到 $0 \dots 7$，或入 `all_mask`。
2. 初始化 `dp` 全为 `inf`，`dp[0][0][8] = 0`。
3. 依次扫描每本书 $h$，用滚动数组更新“抽走”与“保留”两种转移。
4. 枚举所有 $0 \le j \le k,\ \text{msk} \in [0, 255],\ \text{las} \in [0, 8]$，用 `dp[j][msk][las] + __builtin_popcount(all_mask ^ msk)` 更新最小混乱度并按格式输出。

### 4.3 复杂度分析

- **时间复杂度**：单组数据状态数 $k \times 256 \times 9 \approx 2.3 \times 10^5$，扫描 $n$ 本书仅扫描可达状态，单组耗时 $< 15\text{ ms}$。
- **空间复杂度**：两个 $105 \times 256 \times 9$ 的滚动数组，空间复杂度约 $2\text{ MB}$。

---

## 5. 示例代码

```cpp
//
// 题目：P1777 [ICPC 2009 Wuhan R] 帮助
// https://www.luogu.com.cn/problem/P1777
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int inf{0x3f3f3f3f};

int n,k;
int a[105];
int dp[105][256][9],ndp[105][256][9];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int tc=0;
	while(cin>>n>>k&&(n||k))
	{
		int all_mask=0;
		for(int i=0;i<n;++i)
		{
			cin>>a[i];
			a[i]-=25;
			all_mask|=(1<<a[i]);
		}

		for(int j=0;j<=k;++j)
			memset(dp[j],0x3f,sizeof(dp[j]));
		dp[0][0][8]=0;

		for(int i=0;i<n;++i)
		{
			int h=a[i];
			int lim=min(i+1,k);
			for(int j=0;j<=lim;++j)
				memset(ndp[j],0x3f,sizeof(ndp[j]));

			for(int j=0;j<=min(i,k);++j)
				for(int msk=0;msk<256;++msk)
					for(int las=0;las<=8;++las)
					{
						int v=dp[j][msk][las];
						if(v>=inf)
							continue;
						if(j+1<=k)
							ndp[j+1][msk][las]=min(ndp[j+1][msk][las],v);
						int nmsk=msk|(1<<h);
						int nv=v+(las!=h);
						ndp[j][nmsk][h]=min(ndp[j][nmsk][h],nv);
					}
			for(int j=0;j<=lim;++j)
				memcpy(dp[j],ndp[j],sizeof(dp[j]));
		}

		int ans=inf;
		for(int j=0;j<=k;++j)
			for(int msk=0;msk<256;++msk)
			{
				int extra=__builtin_popcount(all_mask^msk);
				for(int las=0;las<=8;++las)
					if(dp[j][msk][las]<inf)
						ans=min(ans,dp[j][msk][las]+extra);
			}
		cout<<"Case "<<++tc<<": "<<ans<<"\n\n";
	}
	return 0;
}
```
