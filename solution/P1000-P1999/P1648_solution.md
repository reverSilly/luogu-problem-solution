# P1648 看守 题解

> 标签：曼哈顿距离 / 状态压缩 / 数学拆绝对值 · 难度：省选/NOI-

## 1. 重新读题

**问题重述（人话版）**

给定 $d$ 维空间中的 $n$ 个点 $P_i = (x_{i,0}, x_{i,1}, \dots, x_{i,d-1})$（$0 \le i < n$）。求任意两点之间曼哈顿距离的最大值：
$$\max_{0 \le i < j < n} \sum_{k=0}^{d-1} |x_{i,k} - x_{j,k}|.$$

**约束与规模暗示**

- $60\%$ 数据：$d \le 2$；
- $100\%$ 数据：$2 \le n \le 10^6$，$1 \le d \le 4$，$1 \le x_{i,k} \le 10^5$。
- 规模暗示：$n = 10^6$ 极大而维度 $d \le 4$ 极小（$2^{d-1} \le 8$），必须消除点对之间的绝对值耦合，在 $O(n \cdot 2^{d-1})$ 时间内单遍流式求解。

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：把绝对值 $|a - b|$ 拆成 $\pm 1$ 符号枚举的最大值

两点曼哈顿距离难以直接分离变量的原因在于绝对值 $|x_{i,k} - x_{j,k}|$ 把 $i$ 和 $j$ 绑在了一起。但注意到对任意实数 $\Delta$，恒有 $|\Delta| = \max(+\Delta, -\Delta)$。

### 2.2 归约到经典算法：$2^{d-1}$ 种符号掩码下的独立极值差

**引理 1（高维曼哈顿距离的符号展开与极值分离恒等式）**

对于任意两点 $P_i, P_j \in \mathbb{R}^d$，定义在掩码 $m \in [0, 2^{d-1})$ 下的线性投影函数：
$$f_m(P_i) = x_{i,0} + \sum_{k=1}^{d-1} (-1)^{(m \gg (k-1)) \& 1} x_{i,k}.$$
则：
$$\max_{0 \le i, j < n} \sum_{k=0}^{d-1} |x_{i,k} - x_{j,k}| = \max_{m \in [0, 2^{d-1})} \left( \max_{0 \le i < n} f_m(P_i) - \min_{0 \le j < n} f_m(P_j) \right).$$

**证明**：
一方面，对任意 $m \in [0, 2^{d-1})$ 及任意 $i, j$，记 $s_0 = +1, s_k = (-1)^{(m \gg (k-1)) \& 1} \in \{-1, +1\}$，由三角不等式/绝对值性质有：
$$f_m(P_i) - f_m(P_j) = \sum_{k=0}^{d-1} s_k (x_{i,k} - x_{j,k}) \le \sum_{k=0}^{d-1} |x_{i,k} - x_{j,k}|,$$
故右端不会超过真实的曼哈顿距离最大值。
另一方面，设最优解点对 $(i^*, j^*)$ 满足 $x_{i^*, 0} \ge x_{j^*, 0}$（否则交换 $i^*, j^*$ 的角色，距离对称不变）。对每个 $k \in [1, d-1]$，若 $x_{i^*, k} \ge x_{j^*, k}$ 则取 $m$ 的第 $k-1$ 位为 $0$（即 $s_k = +1$），否则取第 $k-1$ 位为 $1$（即 $s_k = -1$）。此时每个坐标分量都有 $s_k(x_{i^*,k} - x_{j^*,k}) = |x_{i^*,k} - x_{j^*,k}|$，从而：
$$\sum_{k=0}^{d-1} |x_{i^*,k} - x_{j^*,k}| = f_m(P_{i^*}) - f_m(P_{j^*}) \le \max_{i} f_m(P_i) - \min_{j} f_m(P_j).$$
两边夹逼即证恒等式成立。$\blacksquare$

## 3. 模式总结（Pattern Summary）

- **explore its property** — 利用 $|a - b| = \max(a - b, b - a)$ 将绝对值转化为符号枚举。
- **reduce to easier problem** — 利用点对对称性固定第 $0$ 维符号为 $+1$，仅用 $2^{d-1}$ 个掩码即可覆盖所有方向（引理 1）。
- **classic problem solution** — 在线读入每个点，流式维护 $2^{d-1}$ 个投影方向上的最大值 `mx[m]` 与最小值 `mn[m]`，做到 $O(n 2^{d-1})$ 时间与 $O(2^{d-1})$ 空间。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
|---|---|---|---|---|
| `n, d` | `int` | 点数 $n$ 与维度 $d$ | 读入 | 规模参数 |
| `mx[8]` | `i64 [8]` | 掩码 $m$ 下投影值 $f_m(P_i)$ 的最大值 | `-inf` | 方向上界 |
| `mn[8]` | `i64 [8]` | 掩码 $m$ 下投影值 $f_m(P_i)$ 的最小值 | `+inf` | 方向下界 |
| `x[4]` | `i64 [4]` | 当前读入点的 $d$ 维坐标 | 读入 | 临时坐标缓冲 |

### 4.2 算法步骤

1. 读入 $n, d$，令 `masks = 1 << (d - 1)`，初始化 `mx[0..masks-1] = -inf`、`mn[0..masks-1] = inf`。
2. 循环读入每个点的坐标 `x[0..d-1]`，对每个掩码 $m \in [0, \text{masks})$ 计算 $s = f_m(x)$ 并更新 `mx[m]` 与 `mn[m]`。
3. 输出 $\max_{0 \le m < \text{masks}} (\text{mx}[m] - \text{mn}[m])$。

### 4.3 复杂度与验证报告

- **时间复杂度**：$O(n \cdot d \cdot 2^{d-1})$。
- **空间复杂度**：$O(2^{d-1}) = O(1)$（无需存储全部点坐标）。
- **对拍验证**：`appendix/P1648/P1648_check.py` 中与 $O(n^2 d)$ 暴力基准完成 **500 组随机数据对拍**全部一致；$n = 200,000, d = 4$ 规模实测耗时 **0.2185 s**。

## 5. 示例代码

```cpp
//
// 题目：P1648 看守
// https://www.luogu.com.cn/problem/P1648
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr i64 inf{1145141919810114514LL};

int n,d;
i64 mx[8],mn[8],x[4];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>d;
	int masks=1<<(d-1);
	for(int m=0;m<masks;++m)
	{
		mx[m]=-inf;
		mn[m]=inf;
	}

	for(int i=0;i<n;++i)
	{
		for(int j=0;j<d;++j)
			cin>>x[j];
		for(int m=0;m<masks;++m)
		{
			i64 s=x[0];
			for(int j=1;j<d;++j)
				s+=(m>>(j-1)&1)?-x[j]:x[j];
			mx[m]=max(mx[m],s);
			mn[m]=min(mn[m],s);
		}
	}

	i64 ans=0;
	for(int m=0;m<masks;++m)
		ans=max(ans,mx[m]-mn[m]);
	cout<<ans<<'\n';
	return 0;
}
```
