# P1293 班级聚会 — 题解

## Part 1: 重述问题

一条铁路线（数轴）上有若干城市，莫斯科在端点处（距离 0）。每个城市有 $a_i$ 位同学，到莫斯科的距离为 $d_i$。选一个城市作为聚会地点，使所有同学单程旅行费用之和最小（每公里 1 卢布）。若费用相同，选离莫斯科近的城市。

输入为多行，每行 `同学人数 距离 城市名`，直到 EOF。最后一行一定是莫斯科（距离 0）。

**数据范围：** 城市数 $\le 150$，距离 $\le 10000$，每城人数 $\le 50$。

---

## Part 2: 思维链（发现过程）

### 第一步：理解问题的结构

所有城市都在一条直线上。如果聚会地点在位置 $x$，总费用为

$$F(x) = \sum_{i} a_i \cdot |x - d_i|$$

这是一个**加权绝对值和**问题。我们的任务就是在这条直线上找到使 $F(x)$ 最小的位置 $x$（且 $x$ 必须是某个城市的坐标 $d_j$）。

先不管"必须是城市"这个约束，先思考：在整条数轴上，$F(x)$ 的最小值在哪里取得？

### 第二步：绝对值和的性质——中位数定理

考虑一个更简单的情形：$n$ 个点 $x_1 \le x_2 \le \cdots \le x_n$，求 $g(x) = \sum_{i=1}^{n}|x - x_i|$ 的最小值。

**引理 1（绝对值和最小化定理）：** 设 $x_1 \le x_2 \le \cdots \le x_n$。则 $g(x) = \sum_{i=1}^{n}|x - x_i|$ 在以下区间取得最小值：
- $n$ 为奇数（$n = 2m+1$）：$g$ 在 $x = x_{m+1}$ 处取唯一最小值（若存在重复点则可能退化为区间）。
- $n$ 为偶数（$n = 2m$）：$g$ 在 $[x_m, x_{m+1}]$ 上恒等于最小值。

**证明：**

$g(x)$ 是凸函数（有限个凸函数之和为凸函数）。对 $x$ 不在任何 $x_i$ 处，

$$g'(x) = \#\{i : x_i < x\} - \#\{i : x_i > x\}$$

在 $x_k$ 与 $x_{k+1}$ 之间（$x_k < x < x_{k+1}$），$x$ 左边恰好有 $k$ 个点（$x_1, \dots, x_k$），右边恰好有 $n-k$ 个点（$x_{k+1}, \dots, x_n$），故

$$g'(x) = k - (n-k) = 2k - n$$

凸函数的最小值点满足：左导数 $\le 0$ 且右导数 $\ge 0$（即 0 属于次梯度）。

- **$n = 2m+1$ 奇：** 在 $x = x_{m+1}$ 处，左导数 $= 2m - (2m+1) = -1$，右导数 $= 2(m+1) - (2m+1) = 1$。次梯度 $[-1, 1]$ 包含 $0$，故 $x_{m+1}$ 是最小值点。由于 $g'(x) < 0$ 当 $x < x_{m+1}$（$2k - n < 0 \Leftrightarrow k < m + 1/2$，即 $k \le m$），$g'(x) > 0$ 当 $x > x_{m+1}$（$k \ge m+1$），故 $x_{m+1}$ 是唯一的最小值点（假设所有 $x_i$ 互异）。

- **$n = 2m$ 偶：** 在 $x = x_m$ 处，左导数 $= 2(m-1) - 2m = -2$，右导数 $= 2m - 2m = 0$。次梯度 $[-2, 0]$ 包含 $0$，$x_m$ 是最小值点。在 $x = x_{m+1}$ 处，左导数 $= 2m - 2m = 0$，右导数 $= 2(m+1) - 2m = 2$。次梯度 $[0, 2]$ 包含 $0$，$x_{m+1}$ 也是最小值点。在 $[x_m, x_{m+1}]$ 内，$g'(x) = 2m - 2m = 0$，故 $g$ 在此区间上为常数，整段 $[x_m, x_{m+1}]$ 都取最小值。

**证毕。**

### 第三步：从无权到加权——加权中位数

我们有权重 $a_i$。将每个城市 $i$ "展开"成 $a_i$ 个位于 $d_i$ 的点，得到一个（多重）点集 $y_1 \le y_2 \le \cdots \le y_S$，其中 $S = \sum a_i$ 为总人数。此时 $F(x) = \sum_{j=1}^{S}|x - y_j|$，恰好是引理 1 的无权形式。

**引理 2（加权中位数定理）：** $F(x) = \sum_i a_i |x - d_i|$ 的最小值点为：
- $S$ 奇：$y_{(S+1)/2}$（第 $(S+1)/2$ 小的点）
- $S$ 偶：$[y_{S/2},\; y_{S/2+1}]$（第 $S/2$ 小与第 $S/2+1$ 小的点之间的闭区间）

**证明：** 由引理 1 直接得出，因为 $F(x) = \sum_{j=1}^{S}|x - y_j|$ 就是 $n = S$ 个点的绝对值和。$\blacksquare$

### 第四步：在离散城市上定位最小值点

城市按距离排序：$d_{(1)} \le d_{(2)} \le \cdots \le d_{(n)}$，对应人数 $a_{(1)}, \dots, a_{(n)}$。定义前缀和 $\text{prefix}[j] = a_{(1)} + \cdots + a_{(j)}$，$\text{prefix}[0] = 0$。

展开的点 $y_k$ 落在城市 $d_{(j)}$ 处当且仅当 $\text{prefix}[j-1] < k \le \text{prefix}[j]$。

**引理 3（离散加权中位数）：** 使 $F$ 在离散城市集合上取得最小值的城市（若花费相同取离莫斯科近的）是满足 $\text{prefix}[j] \ge \lceil S/2 \rceil$ 的最小 $j$ 对应的城市 $d_{(j)}$。

**证明：**

- **$S$ 奇，$S = 2m+1$：** 由引理 2，最小值点为 $y_{m+1}$。$y_{m+1}$ 落在城市 $j$ 处 $\Leftrightarrow \text{prefix}[j-1] < m+1 \le \text{prefix}[j]$。即 $\text{prefix}[j] \ge m+1 = \lceil S/2 \rceil$ 且 $\text{prefix}[j-1] < \lceil S/2 \rceil$。这正是"满足 $\text{prefix}[j] \ge \lceil S/2 \rceil$ 的最小 $j$"。

- **$S$ 偶，$S = 2m$：** 由引理 2，最小值区间为 $[y_m, y_{m+1}]$。$y_m$ 落在城市 $j$ 处 $\Leftrightarrow \text{prefix}[j] \ge m$ 且 $\text{prefix}[j-1] < m$。$y_{m+1}$ 落在城市 $j'$ 处 $\Leftrightarrow \text{prefix}[j'] \ge m+1$。由于 $\text{prefix}$ 单调非递减，$j' \ge j$。最小值区间为 $[d_{(j)}, d_{(j')}]$。

  离莫斯科最近的是 $d_{(j)}$（因为 $d_{(j)} \le d_{(j')}$）。而 $j$ 恰好是满足 $\text{prefix}[j] \ge m = \lceil S/2 \rceil$ 的最小下标（因为 $\text{prefix}[j-1] < m = \lceil S/2 \rceil$）。

  特殊情况：若 $j = j'$（$y_m$ 和 $y_{m+1}$ 在同一城市），最小值点唯一。若 $j < j'$（例如 $\text{prefix}[j] = m$ 恰好），则 $d_{(j)}$ 和 $d_{(j+1)}$ 都是最小值点，取 $d_{(j)}$（更靠近莫斯科）。

综合两种情况，结论成立。$\blacksquare$

### 第五步：计算总费用

找到最优城市 $d_{(j)}$ 后，

$$F(d_{(j)}) = \sum_{i} a_i \cdot |d_{(j)} - d_i|$$

直接 $O(n)$ 求和。

**总复杂度：** 排序 $O(n \log n)$ + 求前缀和 $O(n)$ + 求费用 $O(n)$ = $O(n \log n)$。$n \le 150$，完全足够。

---

## Part 3: 模式总结

- **探索问题结构** — 城市在一条直线上，费用为加权绝对值和 $\sum a_i |x - d_i|$
- **简化问题** — 先解决无权情形：$\sum |x - x_i|$ 的最小化由中位数定理给出
- **经典问题** — 加权中位数：把每个点按权重展开，最小值点 = 第 $\lceil S/2 \rceil$ 小的点
- **离散化定位** — 排序后前缀和，找 $\text{prefix}[j] \ge \lceil S/2 \rceil$ 的最小 $j$；偶数 $S$ 时自动偏向莫斯科
- **直接计算** — $O(n)$ 求 $\sum a_i |d_{\text{best}} - d_i|$

---

## Part 4: 正式解法

### 符号表

| 符号 | 含义 | 类型 | 初始化 |
|------|------|------|--------|
| `n` | 城市总数 | `int` | 读入计数 |
| `a[i]` | 城市 $i$ 的信息（`cnt` 人数, `pos` 距离, `nm` 名称） | `struct city` 数组 | 逐行读入，0-based |
| `S` | 总人数 $= \sum_i a[i].cnt$ | `int` | 累加 |
| `T` | 中位数位置阈值 $= \lceil S/2 \rceil$ | `int` | `(S+1)/2` |
| `idx[j]` | 排序后的城市原始下标，`a[idx[0]].pos ≤ a[idx[1]].pos ≤ ...` | `vector<int>` | 按 `pos` 排序 |
| `prefix` | 排序后前 $j$ 个城市的人数之和（运行中累加变量） | `int` | 初始 0 |
| `best` | 最优城市的原始下标 | `int` | 计算 |
| `cost` | 最优城市的总费用 $\sum a[i].cnt \cdot \|a[best].pos - a[i].pos\|$ | `int` | 计算 |

### 算法步骤

1. **读入：** 循环读入 `cnt`, `pos`, `name`，直到 EOF。存入 `a[n]`，`n++`。
2. **排序：** 生成 `idx = [0, 1, ..., n-1]`，按 `a[idx[i]].pos` 升序排序。
3. **求加权中位数：** 计算 $S = \sum a[idx[i]].cnt$，$T = (S+1)/2$（整数除法即 $\lceil S/2 \rceil$）。累加 `prefix`，遍历 $j = 0, 1, \dots, n-1$：`prefix += a[idx[j]].cnt`；若 `prefix >= T`，则 `best = idx[j]`，停止。
4. **计算总费用：** `cost = Σ a[i].cnt * abs(a[best].pos - a[i].pos)`，遍历 $i = 0, \dots, n-1$。
5. **输出：** `a[best].nm << ' ' << cost << '\n'`。

### 复杂度

- 时间：$O(n \log n)$（排序主导）
- 空间：$O(n)$

### 验证

手写验证两组样例：

**样例 1：** 3 城市 A(2人,10km), B(1人,20km), C(2人,30km)。
- $S = 5$, $T = 3$。排序 A,B,C。prefix: 2, 3, 5。$j=1$ 时 prefix=3≥3，best=B(pos=20)。
- cost = 2×|20-10| + 1×|20-20| + 2×|20-30| = 20+0+20 = **40**。输出 `B 40`。

**样例 2：** 4 城市 A(1人,0km), B(1人,10km), C(1人,20km), D(1人,30km)。
- $S = 4$, $T = 2$。prefix: 1, 2, 3, 4。$j=1$ 时 prefix=2≥2，best=B(pos=10)。
- cost = 10+0+10+20 = **40**。
- 交叉验证：F(A)=60, F(B)=40, F(C)=40, F(D)=60。B 和 C 费用相同，B 更靠近莫斯科(10<20)，选 B。正确。

---

## Part 5: 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{155};

struct city{int cnt,pos;string nm;};
city a[mxn];
int n;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int c,d;
	string nm;
	while(cin>>c>>d>>nm)
	{
		a[n]={c,d,nm};
		++n;
	}

	vector<int>idx(n);
	iota(idx.begin(),idx.end(),0);
	sort(idx.begin(),idx.end(),[](int x,int y){return a[x].pos<a[y].pos;});

	int S=0;
	for(int i=0;i<n;++i)
		S+=a[idx[i]].cnt;
	int T=(S+1)/2;

	int prefix=0,best=-1;
	for(int j=0;j<n;++j)
	{
		prefix+=a[idx[j]].cnt;
		if(prefix>=T)
		{
			best=idx[j];
			break;
		}
	}

	int cost=0;
	for(int i=0;i<n;++i)
		cost+=a[i].cnt*abs(a[best].pos-a[i].pos);

	cout<<a[best].nm<<' '<<cost<<'\n';
	return 0;
}
```
