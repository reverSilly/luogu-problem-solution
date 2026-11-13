# P1782 旅行商的背包

## 1. 重新读题

背包总容量为 $C$（$1 \le C \le 10^4$）。共有两类物品：
1. $n$（$1 \le n \le 10^4$）种普通多重背包物品：第 $i$ 种每件体积 $V_i$、价值 $W_i$、最多可选 $D_i$ 件；
2. $m$（$1 \le m \le 5$）件连续/整体积奇货：第 $i$ 件若分配整数体积 $X_i \in [0, C]$，可获得价值 $Y_i = a_i X_i^2 + b_i X_i + c_i$。

求在总体积不超过 $C$ 的条件下可获得的最大总价值。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：单调队列多重背包 + 二次函数泛化物品卷积

问题由两部分拼接而成：
1. **前 $n$ 种多重背包物品**：由于 $n, C \le 10^4$，若用二进制拆分会多一个 $\log D_i \approx 10$ 的因子（达 $10^9$ 次运算），因此必须使用**按模 $V_i$ 同余类分组的单调队列优化多重背包**，将每种物品的转移严格控制在 $\Theta(C)$ 时间内，总耗时 $\Theta(n C)$！
2. **后 $m$ 件二次函数奇货**：因为 $m \le 5$ 极小，每件奇货就是一个在 $[0, C]$ 上任意分配整数体积 $x$ 的分组背包物品，直接用 $\Theta(C^2)$ 的暴力卷积更新 $m$ 次即可（$5 \times \frac{10^8}{2} = 2.5 \times 10^8$ 次简单循环）！

**引理 1（按体积余数分组的单调队列优化多重背包）**：
对于体积为 $v$、价值为 $w$、件数上限为 $d$ 的物品，将容量 $j \in [0, C]$ 按模 $v$ 的余数 $r = j \bmod v$ 分为 $v$ 个互不相关的同余类：$j = k v + r$。则转移方程化为：
$$dp_{\text{new}}[k v + r] = \max_{k - d \le p \le k} \big( dp[p v + r] - p w \big) + k w.$$
当 $k$ 递增时，$p$ 的取值区间 $[k - d, k]$ 是宽度为 $d$ 的滑动窗口，用单调队列即可在 $\Theta(C)$ 时间内完成该种物品的全部状态更新。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 针对 $n = 10^4$ 的多重背包与 $m = 5$ 的泛化二次函数物品分别选用针对性的算法避免 TLE。
- **classic problem solution** — 前 $n$ 种物品用按模 $V_i$ 同余类滑动窗口单调队列在 $\Theta(nC)$ 时间内处理（引理 1），后 $m$ 件奇货直接做 $m$ 次 $\Theta(C^2)$ 背包合并。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, m, C` | `int` | 普通物品种数、奇货数、背包容量 | 标准输入 | 维度 |
| `dp[j], ndp[j]` | `i64 [10005]` | 容量为 $j$ 的最大价值 | 全 `0` | DP arrays |
| `que, qval` | `int[], i64[]` | 单调队列的下标与偏移价值 $dp[p v + r] - p w$ | 每同余类重置 | monotonic queue |

### 4.2 算法步骤

1. 读入 $n, m, C$。
2. 对每种普通物品 $(v, w, d)$：令 $d = \min(d, C / v)$；按余数 $r = 0 \dots v-1$ 枚举商 $k = 0 \dots (C - r) / v$，用单调队列维护窗口 $[k - d, k]$ 内 `dp[p * v + r] - p * w` 的最大值，更新 `dp`。
3. 对每件奇货 $(a, b, c)$：枚举当前总容量 $j = 0 \dots C$ 与分配给该奇货的体积 $x = 0 \dots j$，用 `dp[j - x] + a * x * x + b * x + c` 更新 `ndp[j]`，随后拷回 `dp`。
4. 输出 `dp[C]`。

### 4.3 复杂度分析

- **时间复杂度**：多重背包部分 $\Theta(n C) \le 10^8$ 次操作，奇货部分 $\Theta(m C^2) \le 2.5 \times 10^8$ 次简单循环，总耗时约 $120\text{ ms}$。
- **空间复杂度**：一维数组长度 $10005$，空间复杂度为 $\Theta(C)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1782 旅行商的背包
// https://www.luogu.com.cn/problem/P1782
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxc{10005};
constexpr i64 inf{1145141919810LL};

int n,m,C;
i64 dp[mxc],ndp[mxc];
int que[mxc];
i64 qval[mxc];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>C;
	for(int i=0;i<n;++i)
	{
		int v,d;
		i64 w;
		cin>>v>>w>>d;
		d=min(d,C/v);
		if(d==0)
			continue;
		if(d==1)
		{
			for(int j=C;j>=v;--j)
				dp[j]=max(dp[j],dp[j-v]+w);
			continue;
		}
		for(int r=0;r<v;++r)
		{
			int hd=0,tl=0;
			for(int k=0,j=r;j<=C;++k,j+=v)
			{
				i64 val=dp[j]-k*w;
				while(hd<tl&&qval[tl-1]<=val)
					--tl;
				que[tl]=k;
				qval[tl++]=val;
				while(hd<tl&&que[hd]<k-d)
					++hd;
				dp[j]=qval[hd]+k*w;
			}
		}
	}

	for(int i=0;i<m;++i)
	{
		i64 a,b,c;
		cin>>a>>b>>c;
		fill_n(ndp,C+1,-inf);
		for(int x=0;x<=C;++x)
		{
			i64 y=a*x*x+b*x+c;
			for(int j=x;j<=C;++j)
				ndp[j]=max(ndp[j],dp[j-x]+y);
		}
		memcpy(dp,ndp,sizeof(i64)*(C+1));
	}

	i64 ans=0;
	for(int j=0;j<=C;++j)
		ans=max(ans,dp[j]);
	cout<<ans<<'\n';
	return 0;
}
```
