# P1776 宝物筛选

## 1. 重新读题

给定采集车最大载重 $W$（$0 \le W \le 4 \times 10^4$）和 $n$ 种宝物（$1 \le n \le 100$）。第 $i$ 种宝物每件价值为 $v_i$、重量为 $w_i$、共有 $m_i$ 件（$\sum m_i \le 10^5$）。求在总重量不超过 $W$ 的前提下所能获得的最大总价值。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：多重背包的二进制拆分（Binary Splitting）

如果把第 $i$ 种宝物的 $m_i$ 件逐件拆开跑 0-1 背包，总物品数为 $\sum m_i \le 10^5$，时间复杂度 $O(W \sum m_i) \approx 4 \times 10^9$，必然超时。

**引理 1（正整数的二进制幂次完备表示定理）**：
设正整数 $m$，取最大的整数 $k$ 使得 $1 + 2 + 4 + \dots + 2^{k-1} \le m$，并令余数 $r = m - (2^k - 1)$。则集合
$$B(m) = \{1, 2, 4, \dots, 2^{k-1}\} \cup (r > 0 ? \{r\} : \emptyset)$$
的子集和恰好可以遍历区间 $[0, m]$ 中的**每一个整数**，且 $|B(m)| = \lfloor \log_2 m \rfloor + 1$。

**证明**：
- $B(m)$ 中所有元素之和为 $(2^k - 1) + r = m$，故任何子集和均 $\in [0, m]$。
- 对任意整数 $x \in [0, m]$：
  - 若 $x \le 2^k - 1$，由二进制表示唯一性，$x$ 可由 $\{1, 2, \dots, 2^{k-1}\}$ 的某个子集凑出；
  - 若 $x > 2^k - 1$，则 $0 \le x - r \le m - r = 2^k - 1$，故 $x - r$ 可由 $\{1, 2, \dots, 2^{k-1}\}$ 的某个子集凑出，再加上 $r$ 即得到 $x$。
因此，将 $m_i$ 件相同宝物打包为 $|B(m_i)| = O(\log m_i)$ 件独立 0-1 物品，其可选件数集合与原多重背包完全等价！$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 利用二进制幂加余数集 $\{1, 2, \dots, 2^{k-1}, r\}$ 能拼出 $[0, m_i]$ 全部整数的性质（引理 1）。
- **classic problem solution** — 将每种物品拆分为 $O(\log m_i)$ 个捆绑包后运行标准一维 0-1 背包，复杂度降至 $O(W \sum \log m_i)$。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, W` | `int` | 宝物种数与最大载重 | 标准输入 | 维度 |
| `dp[j]` | `i64 [40005]` | 载重不超过 $j$ 的最大价值 | 全 `0` | knapsack DP |

### 4.2 算法步骤

1. 读入 $n, W$。
2. 对每种宝物 $(v, w, m)$：
   - 若 $w = 0$，直接累加 $v \cdot m$ 到基础收益 `base_ans`；
   - 否则令 $m = \min(m, W / w)$，依次取步长 $c = 1, 2, 4, \dots$（当 $m \ge c$ 时令 $m \gets m - c$）以及剩余 $m > 0$，对每个打包物品 $(c \cdot v, c \cdot w)$ 倒序更新 `dp[W .. c*w]`。
3. 输出 `base_ans + dp[W]`。

### 4.3 复杂度分析

- **时间复杂度**：拆分后总物品数 $\sum \lceil \log_2 m_i \rceil \le 100 \times 14 = 1400$，总时间复杂度为 $O(W \sum \log m_i) \le 4 \times 10^4 \times 1400 \approx 5.6 \times 10^7$ 次加减比较，耗时 $< 25\text{ ms}$。
- **空间复杂度**：一维数组 `dp` 长度 $40005$，空间复杂度为 $\Theta(W)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1776 宝物筛选
// https://www.luogu.com.cn/problem/P1776
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxw{40005};

int n,W;
i64 dp[mxw];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>W;
	i64 base_ans=0;
	for(int i=0;i<n;++i)
	{
		i64 v,w,m;
		cin>>v>>w>>m;
		if(w==0)
		{
			base_ans+=v*m;
			continue;
		}
		m=min(m,W/w);
		for(i64 c=1;m>0;c<<=1)
		{
			i64 cnt=min(c,m);
			m-=cnt;
			i64 cw=cnt*w,cv=cnt*v;
			for(int j=W;j>=cw;--j)
				dp[j]=max(dp[j],dp[j-cw]+cv);
		}
	}
	cout<<base_ans+dp[W]<<'\n';
	return 0;
}
```
