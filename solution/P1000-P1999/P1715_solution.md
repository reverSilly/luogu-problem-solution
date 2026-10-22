# P1715 [USACO16DEC] Lots of Triangles P

## 1. 重新读题

平面上给定 $N$ 个整点（$3 \le N \le 300$，坐标范围 $0 \dots 10^6$），保证**任意三点不共线**。

任意三个点构成一个三角形，共有 $\binom{N}{3}$ 个三角形。若某个三角形严格内部包含 $v$ 个给定的点，则称该三角形的价值为 $v$。对于每个 $v = 0, 1, \dots, N-3$，求价值恰好为 $v$ 的三角形数量。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质与朴素枚举的瓶颈

如果枚举所有 $\binom{N}{3}$ 个三角形，再花 $O(N)$ 检查其余每个点是否在三角形内部，总复杂度是 $O(N^4)$。当 $N = 300$ 时，$\binom{300}{3} \times 300 \approx 1.35 \times 10^9$ 次叉积运算，必然超时。

要在 $O(N^3)$ 时间内解决，必须在枚举每个三角形 $(i, j, k)$ 时用 **$O(1)$ 时间**算出其内部的点数！

经典的想法是“**梯形面积/点数容斥**”：如果所有点的横坐标两两不同，且按横坐标排序为 $x_0 < x_1 < \dots < x_{N-1}$，那么对于任意有向线段 $p_i p_j$（$i < j$），我们可以预处理出横坐标严格介于 $(x_i, x_j)$ 之间且位于线段 $p_i p_j$ 正下方的点数 `under[i][j]`。随后对任意 $i < j < k$，三角形 $\triangle p_i p_j p_k$ 内部的点数就可以通过 `under[i][j]`、`under[j][k]` 与 `under[i][k]` 的加减法 $O(1)$ 得到！

### 2.2 碰壁：如果有两个点横坐标相同（$x_a = x_b$）怎么办？

题目只保证“任意三点不共线”，并**没有保证任意两点横坐标不同**！如果有竖直边或点恰在中间顶点 $p_j$ 的正下方，“严格介于 $(x_i, x_j)$ 之间”就会漏算或重算，分类讨论极其繁琐且极易写错。

能不能不写任何竖直边特判，直接让“按 $(x, y)$ 双关键字排序后的下标顺序”天然等价于严格递增的横坐标？

### 2.3 无穷小剪切变换（Infinitesimal Shear）与容斥公式（附完整证明）

**引理 1（无穷小水平剪切保序与保向引理）**：将所有点按双关键字 $(x, y)$ 升序排序为 $p_0, p_1, \dots, p_{N-1}$。考虑无穷小仿射剪切变换
$$T_\varepsilon(x, y) = (x + \varepsilon y,\ y), \qquad 0 < \varepsilon \ll 1.$$
当 $\varepsilon > 0$ 充分小时：
1. 变换后各点的横坐标 $x'_i = x_i + \varepsilon y_i$ 严格满足 $x'_0 < x'_1 < \dots < x'_{N-1}$；
2. 任意三点 $a, b, c$ 的有向叉积 $\operatorname{cross}(a, b, c) = (b - a) \times (c - a)$ 在变换前后**完全不变**（因而点在三角形内部、在线段上方/下方的关系完全不变）。

**证明**：
1. 对任意 $i < j$，由双关键字排序规则，要么 $x_i < x_j$（此时取 $\varepsilon < \frac{1}{\max |y_a - y_b| + 1}$ 即有 $x'_i < x'_j$），要么 $x_i = x_j$ 且 $y_i < y_j$（此时 $x'_j - x'_i = \varepsilon(y_j - y_i) > 0$）。故 $x'_0 < x'_1 < \dots < x'_{N-1}$ 严格成立。
2. 矩阵 $\begin{pmatrix} 1 & \varepsilon \\ 0 & 1 \end{pmatrix}$ 的行列式为 $1$，对任意三点有
   $$(x'_b - x'_a)(y'_c - y'_a) - (y'_b - y'_a)(x'_c - x'_a) = \big((x_b - x_a) + \varepsilon(y_b - y_a)\big)(y_c - y_a) - (y_b - y_a)\big((x_c - x_a) + \varepsilon(y_c - y_a)\big) = \operatorname{cross}(a, b, c).$$
   叉积恒等不变！这意味着我们**根本不需要在代码里真的算出 $\varepsilon$**：只需按 $(x, y)$ 双关键字排序，下标顺序 $i < m < j$ 就精确等价于剪切平面上的 $x'_i < x'_m < x'_j$，而点 $p_m$ 在有向线段 $p_i \to p_j$ 下方精确等价于原坐标下的 $\operatorname{cross}(p_i, p_j, p_m) < 0$！$\blacksquare$

**引理 2（三角形内部点数的 $O(1)$ 容斥公式）**：按 $(x, y)$ 升序排序后，对任意 $0 \le i < j < N$，定义
$$\text{under}[i][j] = \big|\big\{ m \;\big|\; i < m < j \text{ 且 } \operatorname{cross}(p_i, p_j, p_m) < 0 \big\}\big|.$$
则对任意 $0 \le i < j < k < N$，三角形 $\triangle p_i p_j p_k$ 内部的点数为：
- 若 $\operatorname{cross}(p_i, p_k, p_j) > 0$（即顶点 $p_j$ 位于底边 $p_i p_k$ 上方）：
  $$\text{cnt} = \text{under}[i][j] + \text{under}[j][k] - \text{under}[i][k];$$
- 若 $\operatorname{cross}(p_i, p_k, p_j) < 0$（即顶点 $p_j$ 位于上边 $p_i p_k$ 下方）：
  $$\text{cnt} = \text{under}[i][k] - \text{under}[i][j] - \text{under}[j][k] - 1.$$

**证明**：
在引理 1 的剪切平面上，横坐标严格满足 $x'_i < x'_j < x'_k$，且不存在任何两点同横坐标、不存在三点共线。因此，任何位于 $\triangle p_i p_j p_k$ 内部的点 $p_m$ 必满足 $x'_i < x'_m < x'_k$（即 $i < m < k$ 且 $m \ne j$）。
1. 当 $\operatorname{cross}(p_i, p_k, p_j) > 0$ 时，上边界是折线 $p_i p_j \cup p_j p_k$，下边界是线段 $p_i p_k$。对于任意 $m \in (i, j) \cup (j, k)$：
   - $p_m$ 在上边界下方 $\iff m \in \text{under}[i][j] \sqcup \text{under}[j][k]$；
   - 其中同时也在下边界 $p_i p_k$ 下方的点恰为 $\text{under}[i][k]$（注意 $p_j$ 本身在 $p_i p_k$ 上方，故 $j \notin \text{under}[i][k]$）。
   相减即得内部点数为 $\text{under}[i][j] + \text{under}[j][k] - \text{under}[i][k]$。
2. 当 $\operatorname{cross}(p_i, p_k, p_j) < 0$ 时，上边界是线段 $p_i p_k$，下边界是折线 $p_i p_j \cup p_j p_k$。在 $p_i p_k$ 下方的点集 $\text{under}[i][k]$ 包含了三部分：
   - 严格位于 $\triangle p_i p_j p_k$ 内部的点；
   - 位于下折线 $p_i p_j \cup p_j p_k$ 下方的点（共 $\text{under}[i][j] + \text{under}[j][k]$ 个）；
   - 顶点 $p_j$ 自身（因 $i < j < k$ 且 $\operatorname{cross}(p_i, p_k, p_j) < 0$，故 $j$ 被计入了 $\text{under}[i][k]$）。
   三部分互不相交，故内部点数恰为 $\text{under}[i][k] - \text{under}[i][j] - \text{under}[j][k] - 1$。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 要在 $O(N^3)$ 内统计所有三角形内部点数，需将单次三角形查询降至 $O(1)$。
- **failed attempt（直接按横坐标分段）** — 原平面可能存在竖直边或点在中间顶点正下方，直接按 $x$ 严格不等容斥会漏点。
- **infinitesimal shear reduction** — 证明无穷小水平剪切 $(x+\varepsilon y, y)$ 保持全部叉积不变且使 $(x,y)$ 字典序成为严格横坐标序（引理 1）。
- **classic problem solution** — 预处理有向线段下方点数 `under[i][j]`，对每个三元组 $i < j < k$ 按 $p_j$ 相对于 $p_i p_k$ 的上下位置 $O(1)$ 容斥（引理 2）。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `int` | 点的数量 $N$ | 标准输入 | 维度 |
| `p[i]` | `pt [mxn]` | 按 $(x, y)$ 升序排序后的点坐标 | 读入后排序 | points |
| `under[i][j]` | `int [mxn][mxn]` | 满足 $i < m < j$ 且 $\operatorname{cross}(p_i, p_j, p_m) < 0$ 的点数 | 预处理统计 | points under segment |
| `ans[v]` | `int [mxn]` | 内部恰好含 $v$ 个点的三角形数量 | 全 `0` | histogram |

### 4.2 算法步骤

1. 读入 $N$ 个点的坐标，按 $(x, y)$ 双关键字从小到大排序。
2. 三重循环 $0 \le i < j < N$、$i < m < j$：若 $\operatorname{cross}(p_i, p_j, p_m) < 0$，则 `++under[i][j]`。
3. 三重循环枚举所有三角形 $0 \le i < j < k < N$：
   - 若 $\operatorname{cross}(p_i, p_k, p_j) > 0$，令 `cnt = under[i][j] + under[j][k] - under[i][k]`；
   - 否则令 `cnt = under[i][k] - under[i][j] - under[j][k] - 1`；
   - 令 `++ans[cnt]`。
4. 依次输出 `ans[0], ans[1], ..., ans[n - 3]`。

### 4.3 复杂度分析

- **时间复杂度**：预处理 `under` 与枚举三角形均为 $\binom{N}{3} \approx \frac{N^3}{6}$ 次叉积运算，总时间复杂度为 $\Theta(N^3)$（$N=300$ 时仅需约 $9 \times 10^6$ 次简单整数运算，耗时 $< 15\text{ ms}$）。
- **空间复杂度**：二维数组 `under` 占用 $O(N^2)$ 空间。

---

## 5. 示例代码

```cpp
//
// 题目：P1715 [USACO16DEC] Lots of Triangles P
// https://www.luogu.com.cn/problem/P1715
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{305};

int n;
struct pt{i64 x,y;}p[mxn];
int under[mxn][mxn];
int ans[mxn];

i64 cross(const pt&a,const pt&b,const pt&c)
{
	return (b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>p[i].x>>p[i].y;

	sort(p,p+n,[](const pt&a,const pt&b){
		return a.x!=b.x?a.x<b.x:a.y<b.y;
	});

	for(int i=0;i<n;++i)
		for(int j=i+1;j<n;++j)
			for(int k=i+1;k<j;++k)
				if(cross(p[i],p[j],p[k])<0)
					++under[i][j];

	for(int i=0;i<n;++i)
		for(int j=i+1;j<n;++j)
			for(int k=j+1;k<n;++k)
			{
				int cnt;
				if(cross(p[i],p[k],p[j])>0)
					cnt=under[i][j]+under[j][k]-under[i][k];
				else
					cnt=under[i][k]-under[i][j]-under[j][k]-1;
				++ans[cnt];
			}

	for(int v=0;v<=n-3;++v)
		cout<<ans[v]<<'\n';
	return 0;
}
```
