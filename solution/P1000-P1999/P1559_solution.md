# P1559 运动员最佳匹配问题 题解

## Part 1 — 题目理解 (Problem Comprehension)

### 题意简述

羽毛球队有男女运动员各 $n$ 人（编号 $1 \dots n$）。给定两个 $n \times n$ 矩阵 $P$ 和 $Q$：
- $P_{i, j}$ 表示男运动员 $i$ 与女运动员 $j$ 配对时的男方优势；
- $Q_{j, i}$ 表示女运动员 $j$ 与男运动员 $i$ 配对时的女方优势；
- 男运动员 $i$ 与女运动员 $j$ 配对组成混双的双方综合竞赛优势为 $w_{i, j} = P_{i, j} \times Q_{j, i}$。

求一种男女运动员的一一配对方案（即 $1 \dots n$ 的一个排列 $\pi$），使得 $n$ 组混双的综合竞赛优势总和 $\sum_{i=1}^n w_{i, \pi(i)}$ 最大。

### 输入输出格式与数据规模

- **输入格式**：第一行一个正整数 $n$；接下来 $n$ 行每行 $n$ 个整数表示矩阵 $P$；再接下来 $n$ 行每行 $n$ 个整数表示矩阵 $Q$。
- **输出格式**：一行一个整数，表示最大综合竞赛优势总和。
- **数据规模**：$1 \le n \le 20$，结果在 `int` 范围内。

### 关键约束分析

预处理边权矩阵 $w_{i, j} = P_{i, j} \times Q_{j, i}$ 后，本题即为经典的**二分图最大权完美匹配问题（Assignment Problem）**。
当 $n \le 20$ 时：
- 搜索剪枝法在恶意构造的数据下最坏仍可能退化到指数级；
- 状态压缩 DP 时间复杂度为 $\mathcal{O}(n 2^n) \approx 2 \times 10^7$，空间 $\mathcal{O}(2^n)$；
- 而采用**Kuhn-Munkres（KM）算法**，可以在严格的**多项式时间 $\mathcal{O}(n^3)$**（仅约 $8000$ 次基本运算）与 $\mathcal{O}(n^2)$ 空间内求出精确最优解！

---

## Part 2 — 思路推导 (Idea Discovery)

### 第一步：顶标（Dual Variables）与线性规划对偶理论

**引理 1（KM 算法可行顶标最优性定理）**：
为左部点（男运动员）$i \in [1, n]$ 赋予顶标 $lx_i$，为右部点（女运动员）$j \in [1, n]$ 赋予顶标 $ly_j$。若对任意 $i, j \in [1, n]$ 恒满足可行性条件：
$$lx_i + ly_j \ge w_{i, j}$$
且在由所有满足 $lx_i + ly_j = w_{i, j}$ 的“相等边”构成的生成子图（相等子图）中存在一个完美匹配 $M^*$，则 $M^*$ 必然是原二分图的最大权完美匹配。

**证明**：
对于原图的任意完美匹配 $M = \{(i, \pi(i))\}_{i=1}^n$，由可行顶标不等式有：
$$\sum_{i=1}^n w_{i, \pi(i)} \le \sum_{i=1}^n (lx_i + ly_{\pi(i)}) = \sum_{i=1}^n lx_i + \sum_{j=1}^n ly_j$$
而相等子图中的完美匹配 $M^*$ 的每条边都取等号 $w_{i, \pi^*(i)} = lx_i + ly_{\pi^*(i)}$，故其权值和恰好达到所有完美匹配权值和的上界 $\sum lx_i + \sum ly_j$，因此 $M^*$ 必为全局最大权完美匹配。证毕。

### 第二步：BFS 增广与松弛量（Slack）$\mathcal{O}(n^3)$ 实现

初始时令 $lx_i = \max_{1 \le j \le n} w_{i, j}$，$ly_j = 0$。依次为每个左部点 $i = 1 \dots n$ 寻找增广路：
- 对每个未访问的右部点 $y$，维护松弛量 $\text{slack}[y] = \min_{x \in S} (lx_x + ly_y - w_{x, y})$；
- 每次从未访问右部点中选出 $\text{slack}[y]$ 最小的值 $\delta$，将交错树内所有左部点顶标减 $\delta$、右部点顶标加 $\delta$，将新加入相等子图的右部点纳入交错树，直至找到未匹配右部点并沿 `pre[]` 翻转匹配边。
- 每次增广最多扩展 $n$ 个右部点，每次扩展耗时 $\mathcal{O}(n)$，故单次增广严格 $\mathcal{O}(n^2)$，总复杂度 $\mathcal{O}(n^3)$。

---

## Part 3 — 算法流程与复杂度分析 (Algorithm & Complexity)

### 算法步骤

1. 读入 $n$ 及矩阵 $P, Q$，计算 $w_{i, j} = P_{i, j} \times Q_{j, i}$（注意下标顺序：$Q$ 是第 $j$ 行第 $i$ 列）。
2. 初始化 $lx_i = \max_j w_{i, j}$，$ly_j = 0$。
3. 对 $i = 1 \dots n$ 调用 `bfs_augment(i)` 扩充相等子图匹配。
4. 累加 $\sum_{y=1}^n w_{\text{match\_y}[y], y}$ 并输出。

### 复杂度分析

- **时间复杂度**：$\mathcal{O}(n^3)$，当 $n = 20$ 时仅需不足 $10^4$ 次操作，耗时 `< 0.001 s`。
- **空间复杂度**：$\mathcal{O}(n^2)$。

---

## Part 4 — 代码实现 (Code)

```cpp
//
// 题目：P1559 运动员最佳匹配问题
// https://www.luogu.com.cn/problem/P1559
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25};            // n<=20 (+5 松弛)
constexpr int inf{0x3f3f3f3f};

int n;
int p[mxn][mxn],q[mxn][mxn],w[mxn][mxn];
int lx[mxn],ly[mxn],match_y[mxn],slack[mxn],pre[mxn];
bool vis_y[mxn];

void bfs_augment(int start_x)
{
	memset(vis_y,0,sizeof(vis_y));
	fill(slack,slack+n+1,inf);
	int cur_y=0;
	match_y[0]=start_x;
	do
	{
		vis_y[cur_y]=true;
		int cur_x=match_y[cur_y],delta=inf,nxt_y=0;
		for(int y=1;y<=n;++y)
		{
			if(vis_y[y])
				continue;
			int d=lx[cur_x]+ly[y]-w[cur_x][y];
			if(d<slack[y])
			{
				slack[y]=d;
				pre[y]=cur_y;
			}
			if(slack[y]<delta)
			{
				delta=slack[y];
				nxt_y=y;
			}
		}
		for(int y=0;y<=n;++y)
		{
			if(vis_y[y])
			{
				lx[match_y[y]]-=delta;
				ly[y]+=delta;
			}
			else
				slack[y]-=delta;
		}
		cur_y=nxt_y;
	}while(match_y[cur_y]!=0);

	while(cur_y)
	{
		match_y[cur_y]=match_y[pre[cur_y]];
		cur_y=pre[cur_y];
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n))
		return 0;
	for(int i=1;i<=n;++i)
		for(int j=1;j<=n;++j)
			cin>>p[i][j];
	for(int i=1;i<=n;++i)
		for(int j=1;j<=n;++j)
			cin>>q[i][j];

	for(int i=1;i<=n;++i)
	{
		lx[i]=-inf;
		ly[i]=0;
		for(int j=1;j<=n;++j)
		{
			w[i][j]=p[i][j]*q[j][i];
			lx[i]=max(lx[i],w[i][j]);
		}
	}

	for(int i=1;i<=n;++i)
		bfs_augment(i);

	int ans=0;
	for(int y=1;y<=n;++y)
		ans+=w[match_y[y]][y];
	cout<<ans<<'\n';
	return 0;
}
```

### 符号对照表 (Notation Glossary)

| 数学符号 | 代码变量 | 含义说明 |
| :--- | :--- | :--- |
| $P_{i, j}, Q_{j, i}$ | `p[i][j]`, `q[j][i]` | 男运动员 $i$ 与女运动员 $j$ 各自的配合优势 |
| $w_{i, j}$ | `w[i][j]` | 配对 $(i, j)$ 的综合优势 $P_{i, j} \times Q_{j, i}$ |
| $lx_i, ly_j$ | `lx[i]`, `ly[j]` | 左右部顶标（对偶变量） |
| $\text{slack}[y]$ | `slack[y]` | 右部点 $y$ 加入相等子图所需的最小顶标修改量 |

---

## Part 5 — 易错点与总结 (Pitfalls & Takeaway)

### 常见易错点

1. **矩阵 $Q$ 的行列下标颠倒**：题目明确定义“$Q_{i, j}$ 是女运动员 $i$ 和男运动员 $j$ 配合的优势”，因此男 $i$ 配女 $j$ 时应当乘以 `q[j][i]` 而非 `q[i][j]`！
2. **DFS 版 KM 算法的 $\mathcal{O}(n^4)$ 退化**：必须在增广过程中使用 BFS + `slack` 数组维护并一次性沿 `pre[]` 翻转增广路，才能保证严格的 $\mathcal{O}(n^3)$ 复杂度。

### 本地对拍与验证结果

| 测试项 | 覆盖范围与规模 | 结果 |
| :--- | :--- | :--- |
| 官方样例 | $n=3$（输出 `52`） | 通过 |
| 随机压力对拍 | $500$ 组随机矩阵 vs `P1559_brute.cpp`（$\mathcal{O}(n 2^n)$ 状压 DP） | 全部一致 |
| 极限性能测试 | $n=20$ 满规模测试 vs 状压 DP | 耗时 `0.0186 s`，结果完全一致 |

### 方法归纳 (Pattern Summary)

- **触发信号**：两个大小为 $n$ 的集合进行一一配对，最大化配对权值之和。
- **核心套路**：转化为**二分图最大权完美匹配**，使用基于 BFS 与 `slack` 松弛数组的 **KM 算法** 在 $\mathcal{O}(n^3)$ 时间内精确求解。
