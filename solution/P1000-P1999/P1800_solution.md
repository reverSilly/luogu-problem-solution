# P1800 [ICPC 2005 Tehran R] Software

## 1. 重新读题

公司有 $n$（$1 \le n \le 100$）名技术人员，需要同时开发两个软件，每个软件各包含 $m$（$1 \le m \le 100$）个模块。第 $i$ 名技术人员独立完成软件 $1$ 的一个模块需要 $d_{i,1}$ 天，完成软件 $2$ 的一个模块需要 $d_{i,2}$ 天（$1 \le d_{i,1}, d_{i,2} \le 100$）。同一时刻每人只能做一个模块，求两个软件的全部 $m$ 个模块都完成的最少天数。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：二分交付天数 $T$ + 背包 DP 检验可行性

“最少需要多少天同时完成两项任务”具有天然的单调性：若 $T$ 天内可以完成，则 $T+1$ 天一定也可以完成。

**引理 1（固定总天数 $T$ 下的模块置换 DP 判据）**：
当总天数上限固定为 $T$ 时：
1. 若第 $i$ 名技术人员做了 $k$ 个软件 $1$ 的模块（需耗时 $k \cdot d_{i,1} \le T$），他在剩余时间内最多还能做
   $$\Big\lfloor \frac{T - k \cdot d_{i,1}}{d_{i,2}} \Big\rfloor$$
   个软件 $2$ 的模块；
2. 定义 $dp[j]$ 为前若干名技术人员在合计完成**恰好（或至少）$j$ 个软件 $1$ 模块**（$0 \le j \le m$，超出 $m$ 记为 $m$）的前提下，最多能完成的软件 $2$ 模块总数。则 $T$ 天可行当且仅当处理完 $n$ 个人后有 $dp[m] \ge m$！

**证明**：两个软件的模块计数只需在总和上分别达到 $m$。将软件 $1$ 的完成模块数作为背包容量维度 $j \in [0, m]$，软件 $2$ 的最大完成模块数作为背包价值维度 $dp[j]$，则 $T$ 天内存在分配方案使两软件模块数均 $\ge m$，当且仅当在软件 $1$ 达到 $m$ 时软件 $2$ 的最大可完成数 $dp[m] \ge m$。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 二分总交付时间 $T$，将双目标最小化问题转化为固定时限下的单目标最大化背包问题（引理 1）。
- **classic problem solution** — 在 `check(T)` 中以软件 $1$ 的完成数 $j \in [0, m]$ 为状态，枚举每人做软件 $1$ 的个数转移软件 $2$ 的最大完成数，判定 `dp[m] >= m`。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, m` | `int` | 技术人员数与每软件模块数 | 标准输入 | 维度 |
| `d1[i], d2[i]` | `int [105]` | 第 $i$ 人完成软件 $1$、$2$ 单模块的耗时 | 标准输入 | durations |
| `dp[j], ndp[j]` | `int [105]` | 完成 $j$ 个软件 $1$ 模块时最多完成的软件 $2$ 模块数 | `dp[0]=0`，其余 `-inf` | DP arrays |

### 4.2 算法步骤

1. 读入 $n, m$ 及每人的 $(d_{i,1}, d_{i,2})$。
2. 在区间 $[1, 20000]$ 上二分天数 $T$，用 `check(T)` 检验：
   - 初始化 `dp[0] = 0`，`dp[1 .. m] = -inf`；
   - 对每个人 $i = 0 \dots n-1$：清空 `ndp` 为 `-inf`，枚举此人做软件 $1$ 的模块数 $k = 0 \dots \min(m, T / d_{i,1})$，令其做软件 $2$ 的模块数为 `c2 = (T - k * d1[i]) / d2[i]`，对所有 $j = 0 \dots m$ 用 `dp[j] + c2` 更新 `ndp[min(m, j + k)]`，随后将 `ndp` 拷回 `dp`；
   - 返回 `dp[m] >= m`。
3. 输出二分得到的最小可行天数 $T$。

### 4.3 复杂度分析

- **时间复杂度**：单次 `check(T)` 耗时 $O(n m^2)$（不超过 $100^3 = 10^6$ 次操作），二分 $\le 15$ 次，总时间复杂度为 $O(n m^2 \log(m \max d))$（耗时 $< 10\text{ ms}$）。
- **空间复杂度**：一维数组长度 $105$，空间复杂度为 $\Theta(n + m)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1800 [ICPC 2005 Tehran R] Software
// https://www.luogu.com.cn/problem/P1800
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int inf{0x3f3f3f3f};

int n,m;
int d1[105],d2[105];
int dp[105],ndp[105];

bool check(int T)
{
	fill_n(dp+1,m,-inf);
	dp[0]=0;
	for(int i=0;i<n;++i)
	{
		fill_n(ndp,m+1,-inf);
		int lim=min(m,T/d1[i]);
		for(int k=0;k<=lim;++k)
		{
			int c2=(T-k*d1[i])/d2[i];
			for(int j=0;j<=m;++j)
				if(dp[j]>=0)
				{
					int nj=min(m,j+k);
					ndp[nj]=max(ndp[nj],dp[j]+c2);
				}
		}
		memcpy(dp,ndp,sizeof(int)*(m+1));
	}
	return dp[m]>=m;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>d1[i]>>d2[i];

	int lo=1,hi=20000,ans=20000;
	while(lo<=hi)
	{
		int mid=(lo+hi)>>1;
		if(check(mid))
		{
			ans=mid;
			hi=mid-1;
		}
		else
			lo=mid+1;
	}
	cout<<ans<<'\n';
	return 0;
}
```
